#!/usr/bin/env python3
"""Read-only verification of real, local evidence file hashes and case records.

This verifies file existence, metadata structure, and SHA-256. It cannot prove
that a log's content is truthful or that a game was human-playtested.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import sys
from collections import Counter
from pathlib import Path

SHA256_RE = re.compile(r"^[a-fA-F0-9]{64}$")
STATUSES = {"PASS", "FAIL", "BLOCKED", "NOT RUN", "NOT APPLICABLE"}


def require_string(entry: dict, name: str) -> str:
    value = entry.get(name)
    if not isinstance(value, str) or not value.strip():
        raise ValueError(f"Missing/empty string field: {name}")
    return value.strip()


def checked_path(root: Path, raw: str) -> Path:
    relative = Path(raw)
    if relative.is_absolute() or any(part == ".." for part in relative.parts):
        raise ValueError(f"Unsafe non-relative evidence path: {raw}")
    resolved = (root / relative).resolve(strict=True)
    if not resolved.is_relative_to(root):
        raise ValueError(f"Evidence path escapes authorized root: {raw}")
    if not resolved.is_file():
        raise ValueError(f"Evidence path is not a file: {raw}")
    return resolved


def hash_sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as file:
        for chunk in iter(lambda: file.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def verify_hash(root: Path, path_value: str, sha_value: str) -> tuple[Path, int]:
    if not SHA256_RE.fullmatch(sha_value):
        raise ValueError(f"Expected 64 hex characters for SHA-256: {path_value}")
    path = checked_path(root, path_value)
    actual = hash_sha256(path)
    if actual.lower() != sha_value.lower():
        raise ValueError(f"SHA-256 mismatch for: {path_value}")
    return path, path.stat().st_size


def verify_manifest(root: Path, manifest: dict) -> dict:
    if manifest.get("schema_version") != 1:
        raise ValueError("Expected schema_version=1")
    artifacts = manifest.get("artifacts")
    tests = manifest.get("tests")
    if not isinstance(artifacts, list) or not artifacts:
        raise ValueError("Manifest must list at least one real artifact")
    if not isinstance(tests, list) or not tests:
        raise ValueError("Manifest must list at least one real verification case")

    seen: set[str] = set()
    audited_bytes = 0
    verified_files = 0
    for entry in artifacts:
        if not isinstance(entry, dict):
            raise ValueError("Artifact record must be an object")
        artifact_id = require_string(entry, "id")
        if artifact_id in seen:
            raise ValueError(f"Duplicate manifest ID: {artifact_id}")
        seen.add(artifact_id)
        require_string(entry, "source_ref")
        require_string(entry, "rights_or_license")
        _, size = verify_hash(root, require_string(entry, "path"),
                              require_string(entry, "sha256"))
        audited_bytes += size
        verified_files += 1

    counts: Counter[str] = Counter()
    for entry in tests:
        if not isinstance(entry, dict):
            raise ValueError("Test record must be an object")
        test_id = require_string(entry, "id")
        if test_id in seen:
            raise ValueError(f"Duplicate manifest ID: {test_id}")
        seen.add(test_id)
        status = require_string(entry, "status")
        if status not in STATUSES:
            raise ValueError(f"Invalid status for {test_id}: {status}")
        counts[status] += 1

        if status in {"FAIL", "BLOCKED", "NOT RUN", "NOT APPLICABLE"}:
            require_string(entry, "reason")
        if status in {"PASS", "FAIL"}:
            _, size = verify_hash(root, require_string(entry, "evidence_path"),
                                  require_string(entry, "evidence_sha256"))
            audited_bytes += size
            verified_files += 1
        elif "evidence_path" in entry or "evidence_sha256" in entry:
            _, size = verify_hash(root, require_string(entry, "evidence_path"),
                                  require_string(entry, "evidence_sha256"))
            audited_bytes += size
            verified_files += 1

    return {
        "verified_files": verified_files,
        "audited_bytes": audited_bytes,
        "artifact_records": len(artifacts),
        "test_records": len(tests),
        "case_statuses": dict(sorted(counts.items())),
        "has_open_gates": any(counts[status] > 0 for status in ("FAIL", "BLOCKED", "NOT RUN")),
    }


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Audit real local evidence hashes, no data modification")
    parser.add_argument("--root", type=Path, required=True,
                        help="Existing local directory containing approved evidence")
    parser.add_argument("--manifest", type=Path, required=True,
                        help="Existing JSON manifest referencing files under --root")
    args = parser.parse_args()

    try:
        root = args.root.resolve(strict=True)
        if not root.is_dir():
            raise ValueError("--root must be an existing directory")
        with args.manifest.open("r", encoding="utf-8") as handle:
            manifest = json.load(handle)
        if not isinstance(manifest, dict):
            raise ValueError("Manifest must be a JSON object")
        report = verify_manifest(root, manifest)
    except (OSError, ValueError, KeyError, TypeError, json.JSONDecodeError) as error:
        print(f"EVIDENCE INTEGRITY FAILED: {error}", file=sys.stderr)
        return 1

    print(json.dumps(report, indent=2, sort_keys=True))
    print("Files/hashes verified; human validity, licensing and runtime remain separate gates.")
    if report["has_open_gates"]:
        print("OPEN GATES: FAIL/BLOCKED/NOT RUN cases are present.", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
