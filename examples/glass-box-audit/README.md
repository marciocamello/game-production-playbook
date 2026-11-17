# T20 Example — Read-Only Glass Box Evidence Manifest Audit

**Source:** [audit_evidence_manifest.py](scripts/audit_evidence_manifest.py), an original Python 3.11+ standard-library script.

**Publication status:** The source has been published but **has NOT RUN against a real game manifest** in this repository session. The playbook does not include fabricated game evidence, generated fake PASS results or redistributed private files.

## Reproducible CLI regression tests

[Run the actual 14-case regression harness](tests/test_evidence_auditor.py) with Python 3.11+:

    python ./examples/glass-box-audit/tests/test_evidence_auditor.py

The tests spawn the real auditor against temporary **real files** and real SHA-256 values. They assert the three exit-code classes (0, 1, 2), valid/invalid manifests, incorrect hashes, traversal/symlinks, duplicate IDs, invalid statuses and missing evidence. The symlink case reports **BLOCKED** if the OS does not allow its creation; it is not silently marked PASS.

**Verified locally on 2026-11-17:** Python 3.13.5, Linux sandbox, 14/14 CLI cases PASS, 0 blocked. The original source and tested copies were matched to GitHub blob SHA-1. This validates only the Python audit behavior on that host, **not** actual Unreal gameplay or Windows execution.

## What the script actually does

1. Reads an **existing, nonempty JSON manifest** with schema_version=1, a nonempty artifacts list and nonempty tests list.
2. Requires stable, globally unique artifact/test IDs.
3. For each artifact, requires a real relative evidence file path, SHA-256, source_ref and rights_or_license reference.
4. For each test, requires a valid status: PASS, FAIL, BLOCKED, NOT RUN or NOT APPLICABLE. Requires a reason for unfinished or nonpassing states; PASS and FAIL both require actual file evidence and SHA-256.
5. Resolves evidence paths and refuses path traversal, symlinks escaping the approved root, nonexistent files or hash mismatches.
6. Computes **actual SHA-256** from file bytes and prints counts/status distribution. It never overwrites or edits files.
7. Returns exit **0** for valid file/hash structure with no open FAIL/BLOCKED/NOT RUN gates; **1** for corrupt/invalid evidence; **2** when evidence files validate but open gates remain. **Exit 0 does not authenticate gameplay truth, licensing or human acceptance.**

## Real use

First create real evidence: UBT/UAT logs, actual test reports, authorized screenshots, original asset manifests, license files, packaged build checksums and approved public-safe records. Place permitted copies under an existing local evidence root and record their actual file hashes in a UTF-8 JSON manifest. Never insert fake test results to satisfy the checker.

    python ./examples/glass-box-audit/scripts/audit_evidence_manifest.py --root ./evidence --manifest ./evidence/manifest.json

**Required JSON structure:**

- Top-level: **schema_version** number 1; arrays **artifacts** and **tests** must each be nonempty.
- Artifact record: unique **id**, relative **path**, real **sha256**, nonempty **source_ref**, nonempty **rights_or_license**.
- Test record: unique **id**, valid **status**; a nonempty **reason** for FAIL/BLOCKED/NOT RUN/NOT APPLICABLE; **evidence_path** and **evidence_sha256** for PASS and FAIL and whenever supplied otherwise.
- Do not include personal absolute paths, unrelated private repositories, access credentials or unlicensed redistributed data in any public manifest.

## Limitations

A matching SHA-256 proves the manifest points to unchanged bytes relative to that hash; it cannot establish whether a video demonstrates real gameplay, whether a screenshot is genuine, whether a license is legally sufficient, whether the logs were fabricated, or whether a reviewer approved the result. Apply the human privacy/rights and runtime checks in [T20's complete Glass Box tutorial](../../tutorials/glass-box-provenance-postmortem.md).

The script is an original source example under the repository's [MIT code license](../../LICENSE-CODE.md). No GitHub Actions or hosted automation is used.
