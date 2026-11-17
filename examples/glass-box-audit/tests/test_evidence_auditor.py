"""Real CLI regressions for the original T20 evidence auditor.

Run: python examples/glass-box-audit/tests/test_evidence_auditor.py
Uses disposable real files, actual subprocesses and SHA-256, not fabricated passes.
"""
import copy
import hashlib
import json
import subprocess
import sys
import tempfile
from pathlib import Path

SCRIPT = Path(__file__).resolve().parents[1] / "scripts" / "audit_evidence_manifest.py"


def run_case(name, root, manifest, expected):
    manifest_path = root / "manifest.json"
    manifest_path.write_text(json.dumps(manifest), encoding="utf-8")
    result = subprocess.run(
        [sys.executable, str(SCRIPT), "--root", str(root), "--manifest", str(manifest_path)],
        capture_output=True, text=True, check=False,
    )
    if result.returncode != expected:
        raise AssertionError(
            f"{name}: exit {result.returncode} expected {expected}\n"
            f"{result.stdout}\n{result.stderr}"
        )
    print(f"PASS: {name} (expected exit {expected})")


def main():
    cases = 0
    blocked = 0
    with tempfile.TemporaryDirectory(prefix="gpp-evidence-test-") as temp:
        work = Path(temp)
        root = work / "evidence"
        root.mkdir()
        (root / "artifact.txt").write_text("original test source", encoding="utf-8")
        (root / "pass.log").write_text("real test fixture log", encoding="utf-8")

        def digest(relative):
            return hashlib.sha256((root / relative).read_bytes()).hexdigest()

        artifact = {
            "id": "artifact-001", "path": "artifact.txt",
            "sha256": digest("artifact.txt"),
            "source_ref": "test-evidence-auditor", "rights_or_license": "original test fixture",
        }
        passing = {
            "id": "case-001", "status": "PASS",
            "evidence_path": "pass.log", "evidence_sha256": digest("pass.log"),
        }
        baseline = {"schema_version": 1, "artifacts": [artifact], "tests": [passing]}

        def check(name, mutate, expected):
            nonlocal cases
            manifest = copy.deepcopy(baseline)
            mutate(manifest)
            run_case(name, root, manifest, expected)
            cases += 1

        check("valid-pass", lambda m: None, 0)
        check("not-run-gate",
              lambda m: m.update(tests=[{
                  "id": "case-001", "status": "NOT RUN", "reason": "Engine unavailable"
              }]), 2)

        def fail_with_evidence(m):
            m["tests"][0].update(status="FAIL", reason="Observed rejection")
        check("fail-with-evidence", fail_with_evidence, 2)

        def fail_without_evidence(m):
            fail_with_evidence(m)
            m["tests"][0].pop("evidence_path")
        check("fail-without-evidence", fail_without_evidence, 1)
        check("wrong-hash", lambda m: m["artifacts"][0].update(sha256="0" * 64), 1)
        check("path-traversal", lambda m: m["artifacts"][0].update(path="../artifact.txt"), 1)
        check("duplicate-id",
              lambda m: m["tests"][0].update(id=m["artifacts"][0]["id"]), 1)
        check("unknown-status", lambda m: m["tests"][0].update(status="UNCERTAIN"), 1)
        check("wrong-schema", lambda m: m.update(schema_version=4), 1)
        check("empty-test-list", lambda m: m.update(tests=[]), 1)
        check("missing-file", lambda m: m["artifacts"][0].update(path="missing.txt"), 1)
        check("empty-license-reference",
              lambda m: m["artifacts"][0].update(rights_or_license=""), 1)

        def blocked_case(m):
            m["tests"][0] = {
                "id": "case-001", "status": "BLOCKED", "reason": "Missing Unreal"
            }
        check("blocked-case", blocked_case, 2)

        outside = work / "outside.txt"
        outside.write_text("do not expose", encoding="utf-8")
        try:
            (root / "escape.txt").symlink_to(outside)
        except OSError as error:
            print(f"BLOCKED: symlink test requires OS permission: {error}")
            blocked += 1
        else:
            check("escaping-symlink", lambda m: m["artifacts"][0].update(
                path="escape.txt",
                sha256=hashlib.sha256(outside.read_bytes()).hexdigest()), 1)

    print(f"REAL CLI TESTS: {cases} passed, {blocked} blocked")
    if cases < 13:
        raise RuntimeError("Insufficient verification cases executed")


if __name__ == "__main__":
    main()
