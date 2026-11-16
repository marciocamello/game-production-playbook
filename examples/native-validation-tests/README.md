# T18 Example — Original Native Unreal Automation Test for T07 Snapshot Validation

**Status:** Original real C++ assertions are published. The test has **NOT RUN** through UHT, UBT, Automation in UE 5.9, PIE or a package. No PASS outcomes are asserted by this repository.

**Source:** [GPPDemoSnapshotValidationTest.cpp](Source/GPPDemoSnapshotValidationTest.cpp), an actual IMPLEMENT_SIMPLE_AUTOMATION_TEST registered as **GPP.Persistence.Snapshot.Validation**.

## How it reuses existing code

The test directly exercises [UGPPDemoRunSubsystem::ValidateSnapshot](../savegame-ab/Source/GPPDemoRunSubsystem.cpp) using a transient actual [UGPPDemoRunSaveGame](../savegame-ab/Source/GPPDemoRunSaveGame.h) object. It does **not** duplicate validation algorithms, create a stub state manager, or alter slots on disk.

Eight assertions explicitly evaluate both valid and invalid snapshots: valid Owned; null rejection; unknown schema rejection; duplicate pickup ID rejection; valid Donated; a Recovery ID absent from collected IDs; overlapping Offering/Recovery rejection; and valid Recovered. The test reports all assertions through the Unreal Automation framework.

## Real integration sequence

1. Inspect the project for equivalent tests first. If they exist, integrate missing assertions there and do not duplicate coverage.
2. Confirm T07's classes and headers are accessible from the existing game/test module. Add this one CPP under the approved module source directory and retain its WITH_DEV_AUTOMATION_TESTS guard. No other module/class is needed.
3. Compile with the [real T02 build wrapper](../../tutorials/unreal-project-bootstrap-local-build.md); inspect complete UHT/UBT results.
4. Run the actual test from Unreal Editor's **Session Frontend / Automation**, check its full name and individual assertion output. Verify test-discovery is nonempty.
5. Record each real result and regressions, then proceed to [T18's higher-level Editor, PIE and package gates](../../tutorials/native-tests-editor-pie-profiling.md). A test of value validation alone does not execute disk I/O or make a map playable.

Original educational C++ code is offered under the repository [MIT code license](../../LICENSE-CODE.md). Do not mark an unrun source example as a tested Unreal feature.
