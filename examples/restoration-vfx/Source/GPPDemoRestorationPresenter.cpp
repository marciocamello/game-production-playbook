#include "GPPDemoRestorationPresenter.h"

#include "GPPDemoRunSubsystem.h"
#include "NiagaraComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/GameInstance.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

AGPPDemoRestorationPresenter::AGPPDemoRestorationPresenter()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VisualRoot"));
    SetRootComponent(SceneRoot);

    LeftPiece = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftPiece"));
    CenterPiece = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CenterPiece"));
    RightPiece = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightPiece"));

    // A real, Engine-shipped mesh gives an immediately visible source example.
    // Replace these with approved authored meshes without altering the logic.
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
        TEXT("/Engine/BasicShapes/Cube.Cube"));

    UStaticMeshComponent* Pieces[] =
    {
        LeftPiece.Get(), CenterPiece.Get(), RightPiece.Get()
    };
    const FVector Positions[] =
    {
        FVector(0.0, -100.0, 0.0),
        FVector(0.0, 0.0, 0.0),
        FVector(0.0, 100.0, 0.0)
    };

    for (int32 Index = 0; Index < 3; ++Index)
    {
        UStaticMeshComponent* Piece = Pieces[Index];
        Piece->SetupAttachment(SceneRoot);
        Piece->SetMobility(EComponentMobility::Movable);
        Piece->SetRelativeLocation(Positions[Index]);
        Piece->SetRelativeScale3D(FVector(0.95, 0.95, 1.4));
        Piece->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        Piece->SetGenerateOverlapEvents(false);
        if (CubeMesh.Succeeded())
        {
            Piece->SetStaticMesh(CubeMesh.Object);
        }
    }

    RestorationFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RestorationFX"));
    RestorationFX->SetupAttachment(SceneRoot);
    RestorationFX->SetAutoActivate(false);
}

void AGPPDemoRestorationPresenter::BeginPlay()
{
    Super::BeginPlay();

    UStaticMeshComponent* Pieces[] =
    {
        LeftPiece.Get(), CenterPiece.Get(), RightPiece.Get()
    };
    for (int32 Index = 0; Index < 3; ++Index)
    {
        TargetPositions[Index] = Pieces[Index]->GetRelativeLocation();
    }

    if (UGameInstance* GI = GetGameInstance())
    {
        if (UGPPDemoRunSubsystem* Run = GI->GetSubsystem<UGPPDemoRunSubsystem>())
        {
            Run->OnProgressChanged.AddDynamic(
                this, &AGPPDemoRestorationPresenter::HandleProgressChanged);
            Run->OnBeaconRestored.AddDynamic(
                this, &AGPPDemoRestorationPresenter::HandleFreshBeaconRestored);
        }
    }

    if (BeaconId.IsNone())
    {
        UE_LOG(LogTemp, Error, TEXT("Restoration presenter %s has no BeaconId"),
            *GetName());
    }

    // Always project durable logical state, including actors loaded by World Partition.
    // A loaded SaveGame never plays the one-shot animation.
    SnapToAuthority();
}

void AGPPDemoRestorationPresenter::EndPlay(
    const EEndPlayReason::Type EndPlayReason)
{
    if (UGameInstance* GI = GetGameInstance())
    {
        if (UGPPDemoRunSubsystem* Run = GI->GetSubsystem<UGPPDemoRunSubsystem>())
        {
            Run->OnProgressChanged.RemoveDynamic(
                this, &AGPPDemoRestorationPresenter::HandleProgressChanged);
            Run->OnBeaconRestored.RemoveDynamic(
                this, &AGPPDemoRestorationPresenter::HandleFreshBeaconRestored);
        }
    }

    RestorationFX->Deactivate();
    Super::EndPlay(EndPlayReason);
}

FVector AGPPDemoRestorationPresenter::StartingOffset(const int32 Index) const
{
    switch (Index)
    {
        case 0: return FVector(-170.0, -120.0, 110.0);
        case 1: return FVector(0.0, 0.0, 190.0);
        default: return FVector(170.0, 120.0, 110.0);
    }
}

void AGPPDemoRestorationPresenter::SnapToAuthority()
{
    UGameInstance* GI = GetGameInstance();
    const UGPPDemoRunSubsystem* Run =
        GI ? GI->GetSubsystem<UGPPDemoRunSubsystem>() : nullptr;
    const bool bPowered =
        Run && !BeaconId.IsNone() && Run->IsBeaconPowered(BeaconId);

    bPlaying = false;
    ElapsedSeconds = 0.0f;
    SetActorTickEnabled(false);
    RestorationFX->Deactivate();

    UStaticMeshComponent* Pieces[] =
    {
        LeftPiece.Get(), CenterPiece.Get(), RightPiece.Get()
    };
    for (int32 Index = 0; Index < 3; ++Index)
    {
        Pieces[Index]->SetVisibility(true);
        Pieces[Index]->SetRelativeLocation(
            bPowered ? TargetPositions[Index]
                     : TargetPositions[Index] + StartingOffset(Index));
    }
}

void AGPPDemoRestorationPresenter::HandleProgressChanged()
{
    // Any memory replacement, checkpoint update or new run resolves directly
    // to the latest state. A separate fresh-commit event triggers the VFX.
    SnapToAuthority();
}

void AGPPDemoRestorationPresenter::HandleFreshBeaconRestored(
    const FName RestoredBeaconId)
{
    if (RestoredBeaconId != BeaconId || BeaconId.IsNone() || bPlaying)
    {
        return;
    }

    UGameInstance* GI = GetGameInstance();
    const UGPPDemoRunSubsystem* Run =
        GI ? GI->GetSubsystem<UGPPDemoRunSubsystem>() : nullptr;
    if (!Run || !Run->IsBeaconPowered(BeaconId))
    {
        return;
    }

    bPlaying = true;
    ElapsedSeconds = 0.0f;
    UStaticMeshComponent* Pieces[] =
    {
        LeftPiece.Get(), CenterPiece.Get(), RightPiece.Get()
    };
    for (int32 Index = 0; Index < 3; ++Index)
    {
        Pieces[Index]->SetRelativeLocation(
            TargetPositions[Index] + StartingOffset(Index));
    }

    // Niagara asset is assigned to RestorationFX in an actual Editor Blueprint.
    // The three-piece reconstruction also functions when an FX asset is absent.
    if (RestorationFX->GetAsset())
    {
        RestorationFX->Activate(true);
    }
    SetActorTickEnabled(true);
}

void AGPPDemoRestorationPresenter::Tick(const float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (!bPlaying)
    {
        return;
    }

    const float Duration = FMath::Max(AnimationSeconds, 0.05f);
    ElapsedSeconds = FMath::Min(ElapsedSeconds + FMath::Max(0.0f, DeltaSeconds),
        Duration);
    const float Alpha = FMath::Clamp(ElapsedSeconds / Duration, 0.0f, 1.0f);
    const float Eased = FMath::InterpEaseInOut(0.0f, 1.0f, Alpha, 2.0f);

    UStaticMeshComponent* Pieces[] =
    {
        LeftPiece.Get(), CenterPiece.Get(), RightPiece.Get()
    };
    for (int32 Index = 0; Index < 3; ++Index)
    {
        const FVector Start = TargetPositions[Index] + StartingOffset(Index);
        Pieces[Index]->SetRelativeLocation(
            FMath::Lerp(Start, TargetPositions[Index], Eased));
    }

    if (ElapsedSeconds >= Duration)
    {
        bPlaying = false;
        RestorationFX->Deactivate();
        SetActorTickEnabled(false);
    }
}
