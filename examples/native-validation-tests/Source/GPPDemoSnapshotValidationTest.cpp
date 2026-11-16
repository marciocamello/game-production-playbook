#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "GPPDemoRunSaveGame.h"
#include "GPPDemoRunSubsystem.h"
#include "UObject/Package.h"
#include "UObject/UObjectGlobals.h"

// Use the existing T07 validation function; do not duplicate its business rules.
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FGPPDemoSnapshotValidationTest,
    "GPP.Persistence.Snapshot.Validation",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FGPPDemoSnapshotValidationTest::RunTest(const FString& Parameters)
{
    UGPPDemoRunSaveGame* Snapshot =
        NewObject<UGPPDemoRunSaveGame>(GetTransientPackage());
    if (!TestNotNull(TEXT("Snapshot object exists"), Snapshot))
    {
        return false;
    }

    Snapshot->FormatVersion = UGPPDemoRunSaveGame::CurrentFormatVersion;
    Snapshot->Generation = 0;
    Snapshot->RunId = FGuid::NewGuid();
    Snapshot->MapId = TEXT("TestMap");
    Snapshot->CheckpointId = TEXT("TestCheckpoint");
    Snapshot->CheckpointPosition = FVector(100.0, 200.0, 20.0);

    bool bPassed = true;
    bPassed &= TestTrue(TEXT("Valid owned stage accepted"),
        UGPPDemoRunSubsystem::ValidateSnapshot(Snapshot));
    bPassed &= TestFalse(TEXT("Null snapshot rejected"),
        UGPPDemoRunSubsystem::ValidateSnapshot(nullptr));

    Snapshot->FormatVersion = -1;
    bPassed &= TestFalse(TEXT("Unknown format rejected"),
        UGPPDemoRunSubsystem::ValidateSnapshot(Snapshot));
    Snapshot->FormatVersion = UGPPDemoRunSaveGame::CurrentFormatVersion;

    Snapshot->CollectedFragmentIds.Add(TEXT("Offering01"));
    Snapshot->CollectedFragmentIds.Add(TEXT("Offering01"));
    bPassed &= TestFalse(TEXT("Duplicate pickup ID rejected"),
        UGPPDemoRunSubsystem::ValidateSnapshot(Snapshot));
    Snapshot->CollectedFragmentIds.RemoveAt(1);

    Snapshot->OfferingFragmentId = TEXT("Offering01");
    Snapshot->PoweredBeaconId = TEXT("Beacon01");
    Snapshot->LightStage = EGPPDemoLightStage::Donated;
    bPassed &= TestTrue(TEXT("Committed donated stage accepted"),
        UGPPDemoRunSubsystem::ValidateSnapshot(Snapshot));

    Snapshot->RecoveryFragmentId = TEXT("MissingRecovery");
    Snapshot->LightStage = EGPPDemoLightStage::Recovered;
    bPassed &= TestFalse(TEXT("Uncollected recovery reference rejected"),
        UGPPDemoRunSubsystem::ValidateSnapshot(Snapshot));

    Snapshot->RecoveryFragmentId = TEXT("Offering01");
    bPassed &= TestFalse(TEXT("Offering reused as recovery rejected"),
        UGPPDemoRunSubsystem::ValidateSnapshot(Snapshot));

    Snapshot->CollectedFragmentIds.Add(TEXT("Recovery01"));
    Snapshot->RecoveryFragmentId = TEXT("Recovery01");
    bPassed &= TestTrue(TEXT("Valid recovered stage accepted"),
        UGPPDemoRunSubsystem::ValidateSnapshot(Snapshot));

    return bPassed;
}

#endif // WITH_DEV_AUTOMATION_TESTS
