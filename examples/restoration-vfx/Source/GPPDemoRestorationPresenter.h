#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GPPDemoRestorationPresenter.generated.h"

class UStaticMeshComponent;
class UNiagaraComponent;
class USceneComponent;

// Visual-only restoration. The existing Run Subsystem owns the real beacon,
// ability and save state; this actor never grants progress or controls the gate.
UCLASS(Blueprintable)
class AGPPDemoRestorationPresenter : public AActor
{
    GENERATED_BODY()

public:
    AGPPDemoRestorationPresenter();
    virtual void Tick(float DeltaSeconds) override;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    UPROPERTY(VisibleAnywhere, Category = "Demo|Restoration")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, Category = "Demo|Restoration")
    TObjectPtr<UStaticMeshComponent> LeftPiece;

    UPROPERTY(VisibleAnywhere, Category = "Demo|Restoration")
    TObjectPtr<UStaticMeshComponent> CenterPiece;

    UPROPERTY(VisibleAnywhere, Category = "Demo|Restoration")
    TObjectPtr<UStaticMeshComponent> RightPiece;

    UPROPERTY(VisibleAnywhere, Category = "Demo|Restoration")
    TObjectPtr<UNiagaraComponent> RestorationFX;

    // Must equal the actual Editor-authored ID passed to TryDonateLight.
    UPROPERTY(EditInstanceOnly, Category = "Demo|Restoration")
    FName BeaconId = TEXT("DemoBeacon");

    UPROPERTY(EditAnywhere, Category = "Demo|Restoration", meta = (ClampMin = "0.05"))
    float AnimationSeconds = 2.0f;

    FVector TargetPositions[3];
    float ElapsedSeconds = 0.0f;
    bool bPlaying = false;

    UFUNCTION()
    void HandleProgressChanged();

    UFUNCTION()
    void HandleFreshBeaconRestored(FName RestoredBeaconId);

    void SnapToAuthority();
    FVector StartingOffset(int32 Index) const;
};
