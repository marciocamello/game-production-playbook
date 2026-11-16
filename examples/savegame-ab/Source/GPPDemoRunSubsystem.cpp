#include "GPPDemoRunSubsystem.h"

#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogGPPDemoPersistence, Log, All);

const FString UGPPDemoRunSubsystem::SlotA(TEXT("GPP_Run_A"));
const FString UGPPDemoRunSubsystem::SlotB(TEXT("GPP_Run_B"));

UGPPDemoRunSaveGame* UGPPDemoRunSubsystem::CreateSnapshot()
{
    return Cast<UGPPDemoRunSaveGame>(
        UGameplayStatics::CreateSaveGameObject(UGPPDemoRunSaveGame::StaticClass()));
}

void UGPPDemoRunSubsystem::CopyState(
    UGPPDemoRunSaveGame* Destination, const UGPPDemoRunSaveGame* Source)
{
    check(Destination && Source);
    Destination->FormatVersion = Source->FormatVersion;
    Destination->Generation = Source->Generation;
    Destination->RunId = Source->RunId;
    Destination->MapId = Source->MapId;
    Destination->CheckpointId = Source->CheckpointId;
    Destination->CheckpointPosition = Source->CheckpointPosition;
    Destination->CollectedFragmentIds = Source->CollectedFragmentIds;
    Destination->OfferingFragmentId = Source->OfferingFragmentId;
    Destination->RecoveryFragmentId = Source->RecoveryFragmentId;
    Destination->PoweredBeaconId = Source->PoweredBeaconId;
    Destination->LightStage = Source->LightStage;
}

bool UGPPDemoRunSubsystem::ValidateSnapshot(const UGPPDemoRunSaveGame* S)
{
    if (!S || S->FormatVersion != UGPPDemoRunSaveGame::CurrentFormatVersion
        || S->Generation < 0 || !S->RunId.IsValid() || S->MapId.IsNone()
        || S->CheckpointId.IsNone() || S->CheckpointPosition.ContainsNaN())
    {
        return false;
    }

    TSet<FName> UniqueIds;
    for (const FName Id : S->CollectedFragmentIds)
    {
        if (Id.IsNone() || UniqueIds.Contains(Id))
        {
            return false;
        }
        UniqueIds.Add(Id);
    }

    if ((!S->OfferingFragmentId.IsNone() && !UniqueIds.Contains(S->OfferingFragmentId))
        || (!S->RecoveryFragmentId.IsNone() && !UniqueIds.Contains(S->RecoveryFragmentId))
        || (!S->OfferingFragmentId.IsNone()
            && S->OfferingFragmentId == S->RecoveryFragmentId))
    {
        return false;
    }

    switch (S->LightStage)
    {
        case EGPPDemoLightStage::Owned:
            return S->PoweredBeaconId.IsNone() && S->RecoveryFragmentId.IsNone();
        case EGPPDemoLightStage::Donated:
            return !S->OfferingFragmentId.IsNone()
                && !S->PoweredBeaconId.IsNone()
                && S->RecoveryFragmentId.IsNone();
        case EGPPDemoLightStage::Recovered:
            return !S->OfferingFragmentId.IsNone()
                && !S->PoweredBeaconId.IsNone()
                && !S->RecoveryFragmentId.IsNone();
        default:
            return false;
    }
}

bool UGPPDemoRunSubsystem::CanChangeProgress() const
{
    return ActiveRun && ValidateSnapshot(ActiveRun);
}

bool UGPPDemoRunSubsystem::StartNewRun(
    const FName MapId, const FName CheckpointId, const FVector CheckpointPosition)
{
    if (bSaveInProgress || MapId.IsNone() || CheckpointId.IsNone()
        || CheckpointPosition.ContainsNaN())
    {
        return false;
    }

    UGPPDemoRunSaveGame* NewState = CreateSnapshot();
    if (!NewState)
    {
        return false;
    }

    NewState->RunId = FGuid::NewGuid();
    NewState->MapId = MapId;
    NewState->CheckpointId = CheckpointId;
    NewState->CheckpointPosition = CheckpointPosition;

    if (!ValidateSnapshot(NewState))
    {
        return false;
    }

    ActiveRun = NewState;
    OnProgressChanged.Broadcast();
    return true;
}

bool UGPPDemoRunSubsystem::TryCollectOffering(const FName FragmentId)
{
    if (!CanChangeProgress() || FragmentId.IsNone()
        || ActiveRun->LightStage != EGPPDemoLightStage::Owned
        || !ActiveRun->OfferingFragmentId.IsNone()
        || ActiveRun->CollectedFragmentIds.Contains(FragmentId))
    {
        return false;
    }

    ActiveRun->OfferingFragmentId = FragmentId;
    ActiveRun->CollectedFragmentIds.Add(FragmentId);
    OnProgressChanged.Broadcast();
    return true;
}

bool UGPPDemoRunSubsystem::TryDonateLight(const FName BeaconId)
{
    if (!CanChangeProgress() || BeaconId.IsNone()
        || ActiveRun->LightStage != EGPPDemoLightStage::Owned
        || ActiveRun->OfferingFragmentId.IsNone()
        || !ActiveRun->PoweredBeaconId.IsNone())
    {
        return false;
    }

    ActiveRun->PoweredBeaconId = BeaconId;
    ActiveRun->LightStage = EGPPDemoLightStage::Donated;
    OnProgressChanged.Broadcast();
    return true;
}

bool UGPPDemoRunSubsystem::TryCollectRecovery(const FName FragmentId)
{
    if (!CanChangeProgress() || FragmentId.IsNone()
        || ActiveRun->LightStage != EGPPDemoLightStage::Donated
        || !ActiveRun->RecoveryFragmentId.IsNone()
        || ActiveRun->CollectedFragmentIds.Contains(FragmentId))
    {
        return false;
    }

    ActiveRun->RecoveryFragmentId = FragmentId;
    ActiveRun->CollectedFragmentIds.Add(FragmentId);
    ActiveRun->LightStage = EGPPDemoLightStage::Recovered;
    OnProgressChanged.Broadcast();
    return true;
}

bool UGPPDemoRunSubsystem::TryUpdateCheckpoint(
    const FName CheckpointId, const FVector Position)
{
    if (!CanChangeProgress() || CheckpointId.IsNone() || Position.ContainsNaN())
    {
        return false;
    }

    ActiveRun->CheckpointId = CheckpointId;
    ActiveRun->CheckpointPosition = Position;
    OnProgressChanged.Broadcast();
    return true;
}

bool UGPPDemoRunSubsystem::IsFragmentCollected(const FName FragmentId) const
{
    return ActiveRun && !FragmentId.IsNone()
        && ActiveRun->CollectedFragmentIds.Contains(FragmentId);
}

bool UGPPDemoRunSubsystem::IsBeaconPowered(const FName BeaconId) const
{
    return ActiveRun && !BeaconId.IsNone() && ActiveRun->PoweredBeaconId == BeaconId
        && ActiveRun->LightStage != EGPPDemoLightStage::Owned;
}

EGPPDemoLightStage UGPPDemoRunSubsystem::GetLightStage() const
{
    return ActiveRun ? ActiveRun->LightStage : EGPPDemoLightStage::Owned;
}

FName UGPPDemoRunSubsystem::GetCheckpointId() const
{
    return ActiveRun ? ActiveRun->CheckpointId : NAME_None;
}

FVector UGPPDemoRunSubsystem::GetCheckpointPosition() const
{
    return ActiveRun ? ActiveRun->CheckpointPosition : FVector::ZeroVector;
}

FName UGPPDemoRunSubsystem::GetMapId() const
{
    return ActiveRun ? ActiveRun->MapId : NAME_None;
}

UGPPDemoRunSaveGame* UGPPDemoRunSubsystem::ReadValidSlot(const FString& Slot)
{
    if (!UGameplayStatics::DoesSaveGameExist(Slot, LocalUserIndex))
    {
        return nullptr;
    }

    UGPPDemoRunSaveGame* Candidate = Cast<UGPPDemoRunSaveGame>(
        UGameplayStatics::LoadGameFromSlot(Slot, LocalUserIndex));
    return ValidateSnapshot(Candidate) ? Candidate : nullptr;
}

bool UGPPDemoRunSubsystem::ReadBestSnapshotIntoMemory()
{
    if (bSaveInProgress)
    {
        return false;
    }

    UGPPDemoRunSaveGame* A = ReadValidSlot(SlotA);
    UGPPDemoRunSaveGame* B = ReadValidSlot(SlotB);

    if (A && B && A->Generation == B->Generation)
    {
        UE_LOG(LogGPPDemoPersistence, Error,
            TEXT("Ambiguous equal-generation save slots; refusing to guess."));
        return false;
    }

    UGPPDemoRunSaveGame* Winner = (A && (!B || A->Generation > B->Generation)) ? A : B;
    if (!Winner)
    {
        UE_LOG(LogGPPDemoPersistence, Warning,
            TEXT("No compatible validated save slot is available."));
        return false;
    }

    // This ONLY replaces logical state. The caller must validate the actual
    // map/checkpoint, stream the region, project actors/GAS and enable control.
    ActiveRun = Winner;
    OnProgressChanged.Broadcast();
    return true;
}

bool UGPPDemoRunSubsystem::RequestAsyncSave()
{
    if (bSaveInProgress || !CanChangeProgress())
    {
        return false;
    }

    UGPPDemoRunSaveGame* A = ReadValidSlot(SlotA);
    UGPPDemoRunSaveGame* B = ReadValidSlot(SlotB);

    // Preserve unexpected/corrupted files for explicit recovery rather than
    // destroying both potential backups with a silent new save.
    if (!A && !B
        && (UGameplayStatics::DoesSaveGameExist(SlotA, LocalUserIndex)
            || UGameplayStatics::DoesSaveGameExist(SlotB, LocalUserIndex)))
    {
        UE_LOG(LogGPPDemoPersistence, Error,
            TEXT("Save slots exist but neither validates; preserving both."));
        return false;
    }

    if (A && B && A->Generation == B->Generation)
    {
        UE_LOG(LogGPPDemoPersistence, Error,
            TEXT("Equal generations in save slots; refusing to overwrite."));
        return false;
    }

    const bool bAIsLatest = A && (!B || A->Generation > B->Generation);
    const FString Target = bAIsLatest ? SlotB : SlotA;
    const int64 Latest = FMath::Max(A ? A->Generation : 0LL,
                                   B ? B->Generation : 0LL);
    if (Latest == MAX_int64)
    {
        return false;
    }

    UGPPDemoRunSaveGame* Snapshot = CreateSnapshot();
    if (!Snapshot)
    {
        return false;
    }

    CopyState(Snapshot, ActiveRun);
    Snapshot->Generation = Latest + 1;

    if (!ValidateSnapshot(Snapshot))
    {
        return false;
    }

    // Retain snapshot through async completion; never write directly from a
    // mutable state object that gameplay may continue changing.
    InFlightSnapshot = Snapshot;
    PendingSlot = Target;
    bSaveInProgress = true;

    FAsyncSaveGameToSlotDelegate Callback;
    Callback.BindUObject(this, &UGPPDemoRunSubsystem::OnSaveFinished);
    UGameplayStatics::AsyncSaveGameToSlot(
        InFlightSnapshot, PendingSlot, LocalUserIndex, Callback);
    return true;
}

void UGPPDemoRunSubsystem::OnSaveFinished(
    const FString& SavedSlot, const int32 UserIndex, const bool bSucceeded)
{
    bool bDurable = false;

    if (bSaveInProgress && bSucceeded && UserIndex == LocalUserIndex
        && SavedSlot == PendingSlot && InFlightSnapshot)
    {
        // For small demo snapshots, sync readback is an explicit integrity
        // gate. A successful callback alone does not prove that a schema-valid
        // snapshot can be loaded.
        UGPPDemoRunSaveGame* Readback = ReadValidSlot(PendingSlot);
        bDurable = Readback
            && Readback->Generation == InFlightSnapshot->Generation
            && Readback->RunId == InFlightSnapshot->RunId;

        if (bDurable && ActiveRun && ActiveRun->RunId == InFlightSnapshot->RunId)
        {
            ActiveRun->Generation = InFlightSnapshot->Generation;
        }
    }

    if (!bDurable)
    {
        UE_LOG(LogGPPDemoPersistence, Error,
            TEXT("Async save or readback validation failed; old slot retained."));
    }

    bSaveInProgress = false;
    PendingSlot.Reset();
    InFlightSnapshot = nullptr;
    OnSaveCompleted.Broadcast(bDurable);
}
