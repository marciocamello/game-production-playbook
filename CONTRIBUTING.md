# Contributing to Game Production Playbook

Contributions should help someone produce and verify an actual game. A task name, prompt, or API description is not enough: include the necessary inputs, sequence, observable output, common failures, verification and limitations.

## Review contract

1. Check existing guides, templates and examples before adding another solution. Reuse and amend the canonical owner page.
2. Classify the proposal as **manual**, **AI-assisted**, **AI-driven**, or universally applicable.
3. Specify engine/tool versions for version-sensitive steps; mention alternatives rather than making one vendor mandatory.
4. Distinguish **planned**, **implemented**, **tested**, **human-validated**, and **released** outcomes. Report PASS / FAIL / BLOCKED / NOT RUN with the evidence source.
5. Use **English** for all repository artifacts; conversational discussions may be in other languages.
6. Use **relative repository paths only** in scripts, Markdown, JSON, examples and reports; never embed personal machine roots, home directories, usernames, tokens, private endpoints, credentials or account emails.
7. Do not copy private repository commits, proprietary Engine source, paid assets or restricted media. Verify and record copyright/provenance for any external example.
8. Never edit Unreal `.uasset` or `.umap` bytes directly; use supported Editor tooling.
9. Before assigning agents, define one owner per file/asset and one exclusive operator for a shared Editor/MCP session.
10. Do not submit stubs, empty implementations, fake proof, speculative tool names or screenshots labelled as tests.
11. Do not add GitHub Actions, hosted CI pipelines, background runners or workflow YAML.
12. Prefer small, meaningful, auditable changes with links updated in the README and relevant section index.

## Evidence classes

- **Design evidence:** approved GDD, art brief, diagram or specification.
- **Implementation evidence:** changed source/assets, tool output and asset existence.
- **Static/editor evidence:** successful compilation, import, Editor validation or material/Blueprint checks.
- **Runtime evidence:** actual gameplay, save/load, streaming, input, sound, collision and camera behavior.
- **Release evidence:** a packaged game launched outside the Editor, distribution acceptance and verified public download.

Evidence classes do not substitute for one another.

## Licensing

By contributing original material, you confirm the authority to license original written guides under [CC BY 4.0](LICENSE.md), and any expressly MIT-designated original code under [MIT](LICENSE-CODE.md). Disclose exceptions in [THIRD_PARTY.md](THIRD_PARTY.md). Do not submit copyrighted content you cannot redistribute.

## Case-study ethics

Document failures and scope cuts as clearly as successes. Preserve dates and labels from the specific evidence reviewed; reconcile contradictory historical statuses explicitly rather than rewriting history.