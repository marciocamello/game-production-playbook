#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GPPLightBeacon.generated.h"

class UPointLightComponent;
class USceneComponent;
class UStaticMeshComponent;

// One in-level example beacon. Its visual state is projected by the Character's run state.
UCLASS(Blueprintable)
class AGPPLightBeacon : public AActor
{
    GENERATED_BODY()

public:
    AGPPLightBeacon();

    // Called by the authoritative character after a successful, once-only donation.
    void ApplyPoweredState(bool bPowered);

protected:
    UPROPERTY(VisibleAnywhere, Category = "Demo")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, Category = "Demo")
    TObjectPtr<UStaticMeshComponent> Marker;

    UPROPERTY(VisibleAnywhere, Category = "Demo")
    TObjectPtr<UPointLightComponent> WorldLight;
};
