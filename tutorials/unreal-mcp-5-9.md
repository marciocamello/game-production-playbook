# T01 — Unreal Engine 5.9 MCP: Connect, Discover, Author and Verify

**Tutorial status:** Published and independently reproducible instructions; **no claim of execution on a reader's machine**.

**Primary engine reference:** Unreal Engine **5.9**. Epic currently describes Unreal MCP as **Experimental**. Expect differences in preview builds, custom forks, plugin sets and future releases.

**Audience:** Unreal technical artists, designers, gameplay engineers and authorized AI agents working in a real Unreal Editor session.

**Practical outcome:** connect an authorized MCP client to the correct open Unreal project, inspect available toolsets **without guessing their names**, author or adjust a real warm point light in an explicitly owned working level, save the changed package through the Editor, reload or reopen it, and record truthful verification.

This chapter is about **real Editor execution**, not merely installing a plugin or claiming an MCP endpoint is healthy.

## 1. Source of truth and security

Official sources (reviewed for this tutorial on November 16, 2026):

- [Epic — Unreal MCP in Unreal Editor, Unreal Engine 5.9](https://dev.epicgames.com/documentation/unreal-engine/unreal-mcp-in-unreal-editor?application_version=5.9)
- [Epic — Run External AI Agents in the Unreal Assistant Panel](https://dev.epicgames.com/documentation/unreal-engine/run-external-ai-agents-in-the-unreal-assistant-panel-in-unreal-engine?application_version=5.9)
- [Model Context Protocol — Official specification](https://modelcontextprotocol.io/specification)

**Unreal MCP is an Editor-embedded server.** The Unreal Engine plugin identifier is `ModelContextProtocol`, and the tool-discovery system uses the `ToolsetRegistry` plugin. The friendly UI name is **Unreal MCP**.

The default server is **`http://127.0.0.1:8000/mcp`**, using Streamable HTTP and a local loopback listener. The official documentation states that the server has **no authentication**. Treat the endpoint as trusted local automation with Editor-level write capabilities:

- Keep it bound to loopback. Do **not** expose/port-forward it to the public internet or a shared network.
- Do not add untrusted origins to **Additional Allowed Origins**.
- Do not allow an agent to edit unassigned levels, native `.uasset`/`.umap` bytes, local credentials or unrelated repositories.
- Run one mutating Editor/MCP operator at a time. The server runs tool calls serially on the game thread; multiple independent clients can still make conflicting *editing decisions*.
- Do not enable a runtime MCP server in distributed builds just to support an Editor workflow. Cooked-game MCP has its own `AIRuntimeCallable` rules and attack surface.

**Important:** Unreal MCP and an unrelated third-party MCP plugin can both appear in a project. Inspect actual enabled plugins, ports, servers, tool schemas and active sessions before changing configurations. Do not overwrite an already working human-managed MCP setup merely because this tutorial uses the official Unreal MCP path.

## 2. Preconditions and environment inspection

Work against a **real project you are authorized to edit**. Before changing any asset, identify:

| Requirement | What to inspect |
| --- | --- |
| Unreal version | The version in the running Editor; confirm it is the intended 5.9 build |
| Project identity | Displayed project name, target level and active Editor process |
| Repository state | Current revision, modified files and uncommitted project content |
| Unsaved assets | Dirty packages and current Editor operations |
| Session owner | Who currently controls the Editor, and who will receive the handoff |
| MCP provider | The official Unreal MCP server or an existing compatible third-party server |
| Client | Supported terminal/IDE agent, or external agent through Unreal Assistant |
| Target work | A **real lighting improvement** in a level that the operator owns |

For this tutorial's practical exercise, **do not use an unrelated production level**. Prefer an already approved, assigned level where a warm navigation/key light is genuinely needed. If no appropriate level or authorization exists, perform the read-only connection checks and mark the write exercise **BLOCKED — no authorized target**, rather than creating a dummy map or modifying arbitrary content.

Take note of which assets you must preserve. A dirty Editor session with uncertain file ownership is a reason to halt **writes to those assets**, not a reason to reset the workspace.

## 3. Enable and confirm Unreal MCP

In the running Unreal Editor:

1. Open **Edit → Plugins** and search for **Unreal MCP**. Ensure it is enabled.
2. Search for **All Toolsets** and enable it if you need the complete default tool collection. Alternatively, enable only the specific needed toolset plugins. Unreal MCP depends on **Toolset Registry**.
3. Restart Unreal Editor if prompted.
4. Open **Edit → Editor Preferences → Agentic Workflows → Model Context Protocol**.
5. Check **Auto Start Server** (enabled by default in the referenced 5.9 docs), **Server Port Number** (default `8000`), **Server URL Path** (default `/mcp`) and **Enable Tool Search** (enabled by default).
6. Read **Window → Developer Tools → Output Log** or use the Editor's Output Log search. Look for the server's bind address/port/path and any `LogModelContextProtocol` or `LogToolsetRegistry` errors. Do not treat a checkbox alone as proof of a running server.

If a server is not running despite the required plugins, first inspect the Output Log. The official console commands are:

| Editor console command | Behavior |
| --- | --- |
| `ModelContextProtocol.StartServer [port]` | Start the server, optionally with a port |
| `ModelContextProtocol.StopServer` | Stop it and close sessions |
| `ModelContextProtocol.RefreshTools` | Refresh registered tool providers after a change |
| `ModelContextProtocol.GenerateClientConfig <Client&#124;All>` | Generate a client configuration in the documented location |

When multiple Editors are open, assign a **distinct local port** to each and verify which instance responds before editing anything. Use **Editor Preferences** or the documented `-ModelContextProtocolPort=N` startup flag as appropriate.

### Expected evidence

The Output Log shows a successful local listener. A client connection will later report `serverInfo.name` as `unreal-mcp` and may expose the active project, engine version and process identity in the initialization metadata. **Do not publish engine/project folder paths or local process metadata from private production logs.**

## 4. Choose exactly one client connection route

Both supported routes access the same Editor toolsets. Use the one you actually operate; installing both is unnecessary.

### Route A — Unreal Assistant panel (no MCP config file)

This route is useful when an external agent should operate inside Unreal Editor.

1. Ensure the **Unreal Assistant** plugin is enabled along with Unreal MCP and the toolsets required for your work.
2. Install and sign in to a compatible local agent CLI **and** its required Agent Client Protocol (ACP) adapter, following that agent's documentation. Epic's panel lists Claude Code, Codex, Cursor, OpenCode and Custom ACP agents.
3. Open **Window → Unreal Assistant**.
4. In **Edit → Editor Preferences → General → Unreal Assistant**, expand **External Agent**.
5. Choose the installed agent from the dropdown, enable **Use External Agent**, review **External Agent Status**, and approve the command when the Editor first asks.
6. Select the external agent within the panel and query the **current Editor state**. If the command/adapter is not found, restart the Editor after installation and verify the CLI/adapter is on its `PATH`.
7. Observe the actual tool calls via the panel's **Required Actions** disclosure.

Epic documents these ACP adapter commands for the relevant agents:

```text
Claude Code adapter: claude-agent-acp
Codex adapter:       codex-acp
Cursor CLI:          agent acp
OpenCode CLI:        opencode acp
```

The adapters' installation and compatibility can change; follow the official guide and current vendor instructions before installation. The Unreal Assistant panel starts an ACP agent and connects it to **this Editor's** MCP server automatically, so **do not add a separate `.mcp.json` solely for this route**. Do not put credentials into Custom Agent Command/Arguments: Epic states those values can appear in logs.

### Route B — External IDE or terminal

This route is useful for an agent already running in its own IDE/terminal.

1. Keep the intended project and Editor open with a running MCP server.
2. In **Edit → Editor Preferences → Agentic Workflows → Model Context Protocol**, select your compatible client under **Client Selection**.
3. Choose **Generate Client Config**. In an installed Engine build, the official docs place `.mcp.json` in the **project root**; source builds may generate it in the Engine root.
4. Inspect the generated file before use. Do not overwrite existing client entries or managed configuration without review.
5. Launch or reconnect your client from the location in which it can resolve its configuration.
6. Check the connection to the **intended Editor**, not just whether some MCP server responded.

For clients that use the same `mcpServers` format, this is the documented single-Editor structure (use the Editor-generated file when available):

```json
{
  "mcpServers": {
    "unreal-mcp": {
      "type": "http",
      "url": "http://127.0.0.1:8000/mcp"
    }
  }
}
```

Different clients may use different configuration syntax. Do not assert the same JSON works for every provider. For two Editors, use distinct port names/URLs and **verify the connection identity** before a write. The first client to call the Editor is not automatically the owner of its unsaved packages.

### Optional independent inspector

The official MCP Inspector can diagnose protocol/tool discovery without depending on an AI agent's reasoning. If Node.js/npm is available and the operator approves fetching/running the tool, launch:

```shell
npx @modelcontextprotocol/inspector
```

Select **Streamable HTTP** and connect to `http://127.0.0.1:8000/mcp`. Use read-only discovery before invoking any mutating tool. This command is a documented Inspector launch, **not** a claim that the Inspector was installed or run by this playbook.

## 5. Verify the correct Editor and discover toolsets

Do **not** begin with an unbounded request such as “fix the entire level.” Confirm the server and available tools first.

### A. Verify instance identity

Ask the connected client:

> Identify the running Unreal project, current level, Engine version, active selection, and MCP server identity. Do not write or save anything. Report only non-sensitive identifiers.

The official Unreal MCP `initialize` result includes `_meta["com.epicgames.unrealengine/instance"]` with identity information; the server name is `unreal-mcp`. Verify that the reported project and version are the **intended** ones.

**Gate A:** A correct project and Editor instance are visible to the client. Otherwise do not proceed to writes.

### B. Inspect tool-discovery mode

With **Enable Tool Search** enabled in UE 5.9, `tools/list` normally advertises **four** discovery meta-tools rather than the complete hundreds-of-tools catalog:

| Exact meta-tool | Intended usage |
| --- | --- |
| `list_toolsets` | List registered toolsets and descriptions |
| `describe_toolsets` | Describe one/more named toolsets; `include_schemas: true` includes schemas |
| `describe_tools` | Obtain complete schemas for specific `Toolset.Tool` names |
| `call_tool` | Execute the chosen tool **using the returned schema** |

**Version warning:** older Unreal releases documented different discovery-tool names and defaults. Do not substitute a 5.8 API from memory in a 5.9 Editor. When search mode is disabled, tool visibility works differently; inspect `tools/list` rather than assuming either mode.

Perform a real discovery sequence:

1. Invoke `list_toolsets` and inspect the names.
2. Choose the actual **Editor actor inspection** toolset from its returned description.
3. Invoke `describe_toolsets` for that *returned* toolset with schemas enabled.
4. Identify a **read-only** actor/selection query and call it using its returned schema.
5. Record the query result and compare it with the Editor's selected actor and open level.

**Gate B:** A read-only MCP query matches the visible Editor. If the actor-query tool is not available, report the missing capability rather than inventing `GetSelectedActors` or another undocumented endpoint.

## 6. Execute a real, bounded lighting task

**Ownership requirement:** only the designated Editor operator may author or save the target level during this task. Choose a level that genuinely needs a warm visual cue near an approved landmark, interaction or path.

### Approve the specific desired result

Before any write, document:

- **Level:** exact currently open, authorized package.
- **Anchor:** exact selected Actor near which the light belongs.
- **Visual goal:** a warm-white Point Light that makes a real navigation/interaction cue legible at the target gameplay camera distance, while respecting established art direction.
- **Reuse constraint:** inspect nearby Point Lights first; configure an existing suitable one instead of placing a duplicate.
- **Write scope:** only the approved level and its directly necessary authored lighting assets. Do not modify unrelated gameplay Blueprints, input bindings, SaveGame, widgets or source.
- **Save authority:** only approved packages; preserve unrelated dirty packages.
- **Verification:** query actual Actor properties, review resulting light in the Editor, save, reopen/refresh and re-query.

### One consolidated execution instruction for the operator

The following is **one task**, not several contradictory prompts:

> You are the sole authorized Unreal Editor/MCP operator for the current lighting task. First confirm the Unreal Engine version, the correct project/level/selected Actor, available MCP toolsets, and the existing nearby Point Lights using real read-only tools. Do not guess tool or parameter names. If an existing light can fulfill the approved warm path cue, adjust it; otherwise create one **real Point Light** above the selected anchor Actor in the assigned level. Use the available tool schemas to set a clearly warm-white light and an appropriate local influence; do not change unrelated gameplay state. Check for duplicate lights, confirm the actual Actor transform/type/properties with Editor/MCP reads, and request human review of the resulting in-Editor image at gameplay camera framing. Save only your owned package through supported Unreal Editor tools, verify it survives reopening or reloading the level, and produce a report containing the actual changed package, operations, checks, PASS/FAIL/NOT RUN results, errors, preserved dirty assets, and remaining human or packaged tests. If the toolset cannot perform a required operation, report the limitation and use a supported **real Editor operation** only after owner approval; never pretend the MCP call succeeded. Do not create stubs, touch private project data unrelated to this task, overwrite unknown unsaved work, or claim PIE/Shipping acceptance without execution.

This example adapts an exercise Epic documents (“Add a point light above the selected actor and make it warm white”) into a **fully owned production task** with durable output and inspection. The task does not mandate a particular tool invocation because tool schemas depend on the actual enabled toolsets and Engine build.

### Implementation and inspection sequence

1. Confirm the selected anchor Actor and inspect current lights.
2. Identify an available toolset for **creating or editing an Actor**; retrieve its real schema.
3. Use it to edit/create the approved Point Light. If the exposed toolset lacks needed property controls, select a supported Editor method and record the fallback honestly.
4. Read the resulting Actor's class, transform, color/temperature or equivalent lighting values, mobility/attenuation where relevant, and level ownership.
5. Check that the result is in the **assigned level**, provides a visible cue and has not altered gameplay/collision.
6. Inspect the game-camera view in the Editor; get human approval of lighting quality.
7. Save **only** packages owned by this task. Reopen/reload the relevant level and query again.
8. Record the saved package's repository-relative identifier, test outcomes and any untested runtime behavior.

**Do not assert numeric lighting values as a universal prescription**: exposure, lumen/candela interpretation, source radius and visual quality depend on the level, renderer and art bible. Real inspection and approval determine usable values.

## 7. Acceptance matrix

These are the **required evidence conditions**, not pre-filled execution results.

| Check | How to verify | Evidence needed |
| --- | --- | --- |
| MCP listener | Editor Output Log and client handshake | Bound loopback address and successful connection |
| Correct instance | MCP initialization metadata and Editor title | Matching project, Engine version and intended Editor |
| Read-only discovery | Tool discovery + actor selection query | Returned names, schemas and matching selected Actor |
| Bounded modification | Compare existing lights before/after | Real created or updated light, no duplicate or unrelated edits |
| Visual result | Human reviews target gameplay framing in Editor | Warm local cue appropriate to art direction |
| Save/reopen | Reopen or reload level and re-query properties | Actor and lighting changes still present |
| Conflicts | Check previously dirty assets and changed paths | No unrelated packages overwritten |
| Handoff | Agent/operator report | Actual tool calls, asset, failures and acceptance gaps |
| PIE/packaged runtime | Only if specifically required by the game feature | Separately recorded real gameplay result; otherwise **NOT RUN** |

**Completion rule for T01 practical exercise:** the intended light is **actually authored, saved, reloaded and visually accepted** in an authorized level. Successful MCP setup alone satisfies only the connection part. This repository has **not run these checks on your Editor**; users must populate their own execution reports.

## 8. Troubleshooting and recovery

| Symptom | Likely cause | Non-destructive action |
| --- | --- | --- |
| Client cannot connect | Plugin disabled, no server, port conflict, wrong URL or stale client | Check `LogModelContextProtocol`, plugins, port/path and reconnect |
| Wrong project visible | Multiple Editors using different ports or stale config | Check initialization identity; do not write until correct |
| No useful tools | All Toolsets disabled, wrong plugin set, tool-search mode misunderstood | Inspect `list_toolsets`, enable only required toolsets, refresh/reconnect |
| Tool name/schema error | Toolset changed, guessed name, wrong Engine version | Read `describe_toolsets` and `describe_tools`, then retry the **correct** schema |
| Newly added C++ tool missing | Live Coding did not register new reflected function | Restart Editor when adding `UFUNCTION`; refresh and reconnect |
| One call hangs or agent times out | Long operation or client timeout | Break work into bounded calls; use supported MCP progress/cancellation features as documented |
| Lighting changed but is gone after restart | Asset was not saved, wrong level edited or save failed | Inspect package ownership and save validation; save/reopen properly |
| Another agent changed the same level | Concurrent Editor operator or dirty-package collision | Stop writes, preserve both states, reconcile with human owner |
| Agent says PASS but scene is wrong | Tests ran only against tool responses, not visual scene | Compare actual Editor properties and obtain human inspection |
| Attempt to reach MCP over the internet | Exposed local server or unsafe origin settings | Restore local-only binding, remove unnecessary origins, stop server if idle |

To inspect detailed logs, Epic documents `Log LogModelContextProtocol Verbose` in the Editor console. To reload tools after a change, use `ModelContextProtocol.RefreshTools`. To inspect low-level protocol behavior, use MCP Inspector.

**Do not use “discard unsaved changes” as a routine recovery strategy.** When ownership is uncertain, preserve the work and ask the current Editor owner to resolve the conflict.

## 9. Record the result accurately

For a real execution, record at minimum:

| Field | Fill after execution |
| --- | --- |
| Engine and plugin versions | Actual running versions |
| Operator/session owner | Person/agent controlling the Editor |
| MCP provider / client | Server identity, connection route and client |
| Intended project and level | Public-safe identifier, no private machine path |
| Target Actor | Actual Actor and transform, without sensitive paths |
| Tool discovery | Toolsets and exact tools selected from real schemas |
| Created/changed assets | Actual **repository-relative** content paths |
| Inspection and save/reopen | Observed outcomes with PASS/FAIL/BLOCKED/NOT RUN |
| Human visual review | Reviewer and actual decision, or NOT RUN |
| PIE and standalone | Actual result only when performed; otherwise NOT RUN |
| Errors and corrections | Reproducible failure and real remediation |

A sample format is provided in [the handoff template](../templates/agent-handoff.md) and [the test and acceptance plan](../templates/test-plan.md).

## 10. Next learning step

Once T01 is **actually verified in your own Editor**, apply the exact same discovery, ownership, authoring and verification process to a real **native-derived Blueprint, material instance, UMG widget or placement task**. Each new category needs its own relevant tool schema and acceptance criteria; a passing Point Light task does not prove Animation Graph, MetaSound, Chaos or other tool coverage.

For implementing the first C++/Blueprint gameplay feature, follow [feature execution](../docs/06-production-planning/feature-execution.md). For session rules, follow [AI-driven production](../pipelines/ai-driven-development/README.md). For the historical risk examples, see [The Unmade failures and fixes](../case-studies/the-unmade/failures-and-fixes.md).
