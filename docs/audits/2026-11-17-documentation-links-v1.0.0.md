# Documentation Link Review — v1.0.0

**Review date:** 2026-11-17
**Scope:** All 76 Markdown documents present at the start of this release review, and the full final set of 78 Markdown documents after adding this audit and release notes, including T01–T20, the 197-step roadmap, examples, templates, licenses/provenance and case study.
**Release edition:** [Game Production Playbook v1.0.0](../releases/v1.0.0.md) — **documentation release only**.
**Method:** inspect exact Markdown source on public GitHub; inventory external HTTP(S) references; inspect known local fragments; independently retrieve/search authoritative external pages using a browser/search tool; correct confirmed outdated or unreliable destinations. This was not a continuous network/HTTP-status monitor.

## Result and evidence levels

- **Published scope:** all 20 tutorial pages and the 197-item roadmap remain present.
- **Internal paths:** after v1.0.0 release notes, audit links and index updates were committed, the final GitHub tree was independently re-read at commit `a9344d31a9e1a8d3278393174a9d4c229757bf8a`. **78/78 Markdown documents and 578 relative Markdown file/directory references** were checked against all current GitHub tree paths: **0 unresolved local paths**. This was a source-level path-resolution audit, not a rendered-browser click test or external HTTP-status monitor. The earlier [static source audit](2026-11-17-static-source-audit.md) separately verified its own baseline.
- **Internal Markdown fragments:** one explicit repository-relative anchor was found in the reviewed Markdown source: `examples/savegame-ab/README.md` links to `tutorials/savegame-reload-world-partition.md#11-exact-humanautomated-acceptance-scenarios`. The target heading `## 11. Exact human/automated acceptance scenarios` exists and generates that GitHub-style anchor.
- **External authoritative documentation:** relevant documentation for Unreal Engine, Blender, Git, the Model Context Protocol, Creative Commons, OBS and FFmpeg was retrieved or found in the publisher's search-indexed source. **Direct retrieval** and **search-index evidence** are distinguished below; an indexed result does not prove the live server returned HTTP 200 to every client.
- **Confirmed reference fixes:** three links were updated in their real tutorial files (see table).
- **Version caveat:** the public Epic Developer Community can display a different default UE documentation version than the 5.9-based tutorial, and several version-pinned URLs cannot be fetched by every intermediary. This is not proof of API equivalence. The installed UE 5.9 headers, Editor/Python console, enabled plugins and actual compilation remain authoritative for integration.
- **Execution state:** no UE Editor, Blender runtime, game, sound, video capture or Windows packaging was executed by this link audit; runtime checks remain NOT RUN.

## Three applied link corrections

| Tutorial | Previous reference | Updated reference | Rationale |
| --- | --- | --- | --- |
| [T19 Windows Packaging and Release](../../tutorials/windows-packaging-gameplay-capture-release.md) | `/documentation/unreal-engine/project-launcher-in-unreal-engine` | [Using the Project Launcher in Unreal Engine](https://dev.epicgames.com/documentation/unreal-engine/using-the-project-launcher-in-unreal-engine) | Former destination was not retrievable and the official Project Launcher reference exists at the latter path; the new page's title and content were retrieved |
| [T08 Cameras](../../tutorials/camera-rigs-occlusion-enhanced-input.md) | Older version-sensitive deep API path for `SetViewTargetWithBlend` | [Automatic camera control C++ guide](https://dev.epicgames.com/documentation/unreal-engine/quick-start-guide-to-implementing-automatic-camera-control-in-unreal-engine-cpp) | The previous API deep-link could return an empty/unstable page; the official C++ tutorial describes the same actual camera switch call |
| [T09 UMG/MVVM](../../tutorials/umg-mvvm-hud-menus-settings-loading.md) | Older `/documentation/en-us/unreal-engine/API/...` prefix | [UEnhancedPlayerInput::GetEnhancedActionMappingsView](https://dev.epicgames.com/documentation/unreal-engine/API/Plugins/EnhancedInput/UEnhancedPlayerInput/GetEnhancedActionMappingsView) | The canonical API path is indexed by Epic; its documentation explains read-only action mappings |

The corrections change **reference URLs only**. They are not engine code fixes or tests.

## Authoritative sources reviewed

**Retrieved full/partial page content:** Epic's Unreal MCP overview, Automation Test Framework, Functional Testing, Geometry Scripting, UMG Viewmodel, Enhanced Input and GAS overviews, Gameplay Tags, SaveGame APIs, World Partition, BuildCookRun/packaging, FBX pipeline, IK Retargeting, Sequencer, Cine Camera, Niagara, Chaos, MetaSounds, Quartz, sound attenuation, Unreal Insights and Project Launcher; official Model Context Protocol specification; Git documentation; Creative Commons BY 4.0 terms; OBS knowledge base; and ffprobe documentation.

**Indexed in publisher search but not reliably fetchable directly by this browser:** Epic UE 5.9 Preview Release Notes and the UE 5.9 Unreal Assistant external-agent article; the Epic Unreal MCP version-pinned variant; selected Blender 4.5/5.0/5.1 API/manual pages. For the Blender pages, direct retrieval returned **402 from the browsing intermediary** while the publisher's indexed search entries contained matching titles/content. Those links have **not** been classified as broken and no substitute URL was invented.

**Local endpoint:** `http://127.0.0.1:8000/mcp` in T01 is an explicit loopback example, not an external public website. It must not be treated as a dead public reference or exposed over a remote interface.

Some dynamic documentation sites and version parameters depend on client region, cookies, JavaScript, authorization or crawler cache. Do not claim a fully exhaustive 200/404/redirect response-code audit for links when that network evidence is unavailable.

## Release documentation link policy

1. Link to the relevant publisher-controlled page, not an unverified third-party duplicate.
2. Verify that the **page title and topic** match the claim; a root website returning 200 does not validate a deep API link.
3. Prefer stable, current documentation destinations. When pinning a specific version, record why it is necessary and compare with installed tool behavior before changing engine-specific instructions.
4. Keep genuine failures distinguishable from intermediate 402/403/timeout/cache misses and from a documented local loopback URL.
5. Verify repository-relative paths and local heading anchors before publication; never manually edit GitHub-generated IDs without checking the actual heading.
6. Maintain **one canonical version file** and descriptive release notes. A docs release is not an Unreal packaged-game release.
7. Recheck external sites periodically or after a major engine/tool version update, without introducing GitHub Actions or other hosted CI.

**Disposition:** Documentation link review is complete for v1.0.0 with the three documented corrections, one verified internal heading fragment, and transparent external-provider access limitations. **No known confirmed broken external destination from the corrected set remains.** This is not a guarantee that every external destination will remain reachable or that untested external references have returned HTTP 200 from every client.

**No GitHub tag or GitHub Releases object is implied by this report.** They are distinct remote Git objects, and only a `VERSION` file and versioned documentation are authored by this publication workflow.
