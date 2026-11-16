#include "GPPLightBeacon.h"

#include "Components/PointLightComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AGPPLightBeacon::AGPPLightBeacon()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    RootComponent = SceneRoot;

    Marker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Marker"));
    Marker->SetupAttachment(SceneRoot);
    Marker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Marker->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.4f));

    // An actual built-in Unreal mesh, rather than a missing placeholder art asset.
    static ConstructorHelpers::FObjectFinder<UStaticMesh> EngineCylinder(
        TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    if (EngineCylinder.Succeeded())
    {
        Marker->SetStaticMesh(EngineCylinder.Object);
    }

    WorldLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("RestoredWorldLight"));
    WorldLight->SetupAttachment(SceneRoot);
    WorldLight->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
    WorldLight->SetIntensity(1800.0f);
    WorldLight->SetLightColor(FLinearColor(1.0f, 0.72f, 0.36f));
    WorldLight->SetAttenuationRadius(480.0f);
    WorldLight->SetVisibility(false);
}

void AGPPLightBeacon::ApplyPoweredState(const bool bPowered)
{
    WorldLight->SetVisibility(bPowered);
}
