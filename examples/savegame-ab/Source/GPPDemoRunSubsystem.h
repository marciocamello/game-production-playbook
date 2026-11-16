#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GPPDemoRunSaveGame.h"
#include "GPPDemoRunSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGPPDemoProgressChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGPPDemoSaveCompleted, bool, bSucceeded);
// Emitted only on a newly accepted donation; never on SaveGame load or projection.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGPPDemoBeaconRestored, FName, BeaconId);

// Single-player in-memory progression authority. The pawn, ASC, actors and UI
// are projections. Connect the real gameplay transactions to this subsystem.
UCLASS()
class UGPPDemoRunSubsystem final : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "Demo|Progress")
    FGPPDemoProgressChanged OnProgressChanged;

    UPROPERTY(BlueprintAssignable, Category = "Demo|Progress")
    FGPPDemoBeaconRestored OnBeaconRestored;

    UPROPERTY(BlueprintAssignable, Category = "Demo|Save")
    FGPPDemoSaveCompleted OnSaveCompleted;

    UFUNCTION(BlueprintCallable, Category = "Demo|Progress")
    bool StartNewRun(FName MapId, FName CheckpointId, FVector CheckpointPosition);

    UFUNCTION(BlueprintCallable, Category = "Demo|Progress")
    bool TryCollectOffering(FName FragmentId);

    UFUNCTION(BlueprintCallable, Category = "Demo|Progress")
    bool TryDonateLight(FName BeaconId);

    UFUNCTION(BlueprintCallable, Category = "Demo|Progress")
    bool TryCollectRecovery(FName FragmentId);

    UFUNCTION(BlueprintCallable, Category = "Demo|Progress")
    bool TryUpdateCheckpoint(FName CheckpointId, FVector Position);

    UFUNCTION(BlueprintPure, Category = "Demo|Progress")
    bool IsFragmentCollected(FName FragmentId) const;

    UFUNCTION(BlueprintPure, Category = "Demo|Progress")
    bool IsBeaconPowered(FName BeaconId) const;

    UFUNCTION(BlueprintPure, Category = "Demo|Progress")
    EGPPDemoLightStage GetLightStage() const;

    // Save A/B alternates destination slots and reads the written slot back.
    // Never call save/load concurrently. World activation is a separate gate.
    UFUNCTION(BlueprintCallable, Category = "Demo|Save")
    bool RequestAsyncSave();

    // Deserialize and validate logical state only; does NOT stream the map,
    // project world actors, reconcile the ASC or validate physical pawn safety.
    UFUNCTION(BlueprintCallable, Category = "Demo|Save")
    bool ReadBestSnapshotIntoMemory();

    UFUNCTION(BlueprintPure, Category = "Demo|Save")
    bool IsSaveInProgress() const { return bSaveInProgress; }

    UFUNCTION(BlueprintPure, Category = "Demo|Save")
    bool HasActiveRun() const { return ActiveRun != nullptr; }

    UFUNCTION(BlueprintPure, Category = "Demo|Save")
    FName GetCheckpointId() const;

    UFUNCTION(BlueprintPure, Category = "Demo|Save")
    FVector GetCheckpointPosition() const;

    UFUNCTION(BlueprintPure, Category = "Demo|Save")
    FName GetMapId() const;

    static bool ValidateSnapshot(const UGPPDemoRunSaveGame* Candidate);

private:
    static constexpr int32 LocalUserIndex = 0;
    static const FString SlotA;
    static const FString SlotB;

    UPROPERTY(Transient)
    TObjectPtr<UGPPDemoRunSaveGame> ActiveRun;

    UPROPERTY(Transient)
    TObjectPtr<UGPPDemoRunSaveGame> InFlightSnapshot;

    bool bSaveInProgress = false;
    FString PendingSlot;

    static UGPPDemoRunSaveGame* CreateSnapshot();
    static void CopyState(UGPPDemoRunSaveGame* Destination, const UGPPDemoRunSaveGame* Source);
    static UGPPDemoRunSaveGame* ReadValidSlot(const FString& Slot);

    void OnSaveFinished(const FString& SavedSlot, int32 UserIndex, bool bSucceeded);
    bool CanChangeProgress() const;
};
