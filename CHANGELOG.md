# Changelog

## 2026-11-16 — T02: real local Unreal builds and packages

- Published [T02 — Unreal Engine 5.9 project bootstrap and local build pipeline](tutorials/unreal-project-bootstrap-local-build.md).
- Added actual reusable PowerShell wrappers for Engine/project preflight, `Build.bat` Editor compilation and `RunUAT.bat BuildCookRun` Windows packaging in [examples/unreal-local-build/](examples/unreal-local-build/README.md).
- Enforced explicit version/target checks and unique package archives; no GitHub Actions or private machine paths.
- Linked the tutorial and scripts from the main index, tutorial roadmap and Unreal Engine pipeline.
- **Execution note:** wrappers are published as real commands but were not run against a live Windows Unreal 5.9 installation by this repository; build, PIE and packaged-game results remain unverified.

## 2026-11-16 — Practical tutorial series started

- Published the [20-topic technical tutorial roadmap](tutorials/README.md), differentiating authored guidance from topics not yet authored.
- Published [T01: Unreal Engine 5.9 MCP](tutorials/unreal-mcp-5-9.md): safe local setup, client selection, schema discovery, a real bounded lighting exercise, save/reopen checks and troubleshooting.
- Linked the new tutorial to the main README and Unreal Engine pipeline.
- All tutorial content is English; the instructions are referenced against Epic's UE 5.9 documentation. No reader's local Editor was operated or tested by this publication.

## 2026-11-16 — Public playbook foundation

- Initialized independent, public documentation repository with manual, AI-assisted and AI-driven production paths.
- Defined a 197-step production roadmap, evidence-oriented task contracts, and versioned guidance for Unreal Engine and Blender.
- Established CC BY 4.0 documentation and MIT code licensing notices without relicensing third-party content.
- Began a sanitized The Unmade case study based on historical design files and agent implementation reports.
- Added no hosted workflows, application code, binary Unreal assets, private repository history or device-specific paths.

This is a **documentation release**. It does not claim an Unreal game or tooling has been built from this repository.