#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GPPDemoRunSaveGame.generated.h"

UENUM(BlueprintType)
enum class EGPPDemoLightStage : uint8
{
    Owned,
    Donated,
    Recovered
};

// Small, versioned logical snapshot. Never store live Actor pointers or GAS handles.
UCLASS()
class UGPPDemoRunSaveGame final : public USaveGame
{
    GENERATED_BODY()

public:
    static constexpr int32 CurrentFormatVersion = 1;

    UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadOnly, Category = "Demo|Save")
    int32 FormatVersion = CurrentFormatVersion;

    UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadOnly, Category = "Demo|Save")
    int64 Generation = 0;

    UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadOnly, Category = "Demo|Save")
    FGuid RunId;

    UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadOnly, Category = "Demo|Save")
    FName MapId = NAME_None;

    UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadOnly, Category = "Demo|Save")
    FName CheckpointId = NAME_None;

    // Advisory transform only: a real world loader must still validate safe geometry.
    UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadOnly, Category = "Demo|Save")
    FVector CheckpointPosition = FVector::ZeroVector;

    UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadOnly, Category = "Demo|Save")
    TArray<FName> CollectedFragmentIds;

    UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadOnly, Category = "Demo|Save")
    FName OfferingFragmentId = NAME_None;

    UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadOnly, Category = "Demo|Save")
    FName RecoveryFragmentId = NAME_None;

    UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadOnly, Category = "Demo|Save")
    FName PoweredBeaconId = NAME_None;

    UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadOnly, Category = "Demo|Save")
    EGPPDemoLightStage LightStage = EGPPDemoLightStage::Owned;
};
