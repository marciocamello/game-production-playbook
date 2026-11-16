#include "GPPLightCharacter.h"

#include "GPPLightAbility.h"
#include "GPPLightBeacon.h"
#include "GPPLightTags.h"

#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Camera/CameraComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "UObject/ConstructorHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogGPPLightExample, Log, All);

AGPPLightCharacter::AGPPLightCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
    AbilitySystem->SetIsReplicated(false); // This tutorial deliberately targets standalone single-player.

    CharacterMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CharacterMarker"));
    CharacterMarker->SetupAttachment(GetRootComponent());
    CharacterMarker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CharacterMarker->SetRelativeScale3D(FVector(0.6f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> EngineSphere(
        TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    if (EngineSphere.Succeeded())
    {
        CharacterMarker->SetStaticMesh(EngineSphere.Object);
    }

    PersonalLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PersonalLight"));
    PersonalLight->SetupAttachment(GetRootComponent());
    PersonalLight->SetRelativeLocation(FVector(0.0f, 0.0f, 65.0f));
    PersonalLight->SetIntensity(1300.0f);
    PersonalLight->SetLightColor(FLinearColor(0.25f, 0.8f, 1.0f));
    PersonalLight->SetAttenuationRadius(440.0f);
    PersonalLight->SetVisibility(false);

    CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
    CameraArm->SetupAttachment(GetRootComponent());
    CameraArm->TargetArmLength = 560.0f;
    CameraArm->SetRelativeRotation(FRotator(-55.0f, 45.0f, 0.0f));
    CameraArm->SetUsingAbsoluteRotation(true);
    CameraArm->bDoCollisionTest = true;
    CameraArm->ProbeChannel = ECC_Camera;
    CameraArm->ProbeSize = 12.0f;
    // Keep lag off initially to isolate true collision correction from smoothing.
    CameraArm->bEnableCameraLag = false;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
    FollowCamera->SetRelativeLocation(FVector::ZeroVector);
    // Requested zoom is separate from the camera's actual collision-retracted position.
    DesiredCameraArmLength = CameraArm->TargetArmLength;

    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

UAbilitySystemComponent* AGPPLightCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystem;
}

void AGPPLightCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (!ensureMsgf(AbilitySystem != nullptr, TEXT("Demo AbilitySystem is required.")))
    {
        return;
    }

    // Actor-owned ASC for this one-player tutorial. Multiplayer possession/replication
    // requires a different lifecycle and is explicitly outside this example's scope.
    AbilitySystem->InitAbilityActorInfo(this, this);

    if (HasAuthority())
    {
        AbilitySystem->GiveAbility(FGameplayAbilitySpec(UGPPLightAbility::StaticClass(), 1));
    }

    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    ApplyLightStage(EGPPLightStage::Owned);
}

void AGPPLightCharacter::Tick(const float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    const float MinimumLength = FMath::Max(100.0f, CameraMinimumArmLength);
    const float MaximumLength = FMath::Max(MinimumLength, CameraMaximumArmLength);
    DesiredCameraArmLength = FMath::Clamp(DesiredCameraArmLength, MinimumLength, MaximumLength);

    const float UpdatedLength = FMath::FInterpTo(
        CameraArm->TargetArmLength, DesiredCameraArmLength, DeltaSeconds,
        FMath::Max(0.1f, CameraZoomInterpolation));

    CameraArm->TargetArmLength = UpdatedLength;

    const bool bRetracted = CameraArm->IsCollisionFixApplied();
    if (bRetracted != bWasCameraRetracted)
    {
        UE_LOG(LogGPPLightExample, Display,
            TEXT("Camera boom collision: %s, retraction %.1f cm; requested length %.1f cm"),
            bRetracted ? TEXT("applied") : TEXT("cleared"),
            GetCameraRetractionCm(), CameraArm->TargetArmLength);
        bWasCameraRetracted = bRetracted;
    }
}

bool AGPPLightCharacter::IsCameraRetracted() const
{
    return CameraArm && CameraArm->IsCollisionFixApplied();
}

float AGPPLightCharacter::GetCameraRetractionCm() const
{
    if (!CameraArm || !FollowCamera || !CameraArm->IsCollisionFixApplied())
    {
        return 0.0f;
    }

    return FVector::Dist(
        CameraArm->GetUnfixedCameraPosition(), FollowCamera->GetComponentLocation());
}

void AGPPLightCharacter::ResetSprint()
{
    EndSprint();
}

void AGPPLightCharacter::UnPossessed()
{
    EndSprint();
    Super::UnPossessed();
}

void AGPPLightCharacter::PawnClientRestart()
{
    Super::PawnClientRestart();
    ResetSprint();

    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC || !PC->GetLocalPlayer())
    {
        return;
    }

    UEnhancedInputLocalPlayerSubsystem* Subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

    if (Subsystem && MappingContext)
    {
        // Remove this example's own context only; never ClearAllMappings().
        Subsystem->RemoveMappingContext(MappingContext);
        Subsystem->AddMappingContext(MappingContext, 0);
    }
    else
    {
        UE_LOG(LogGPPLightExample, Error, TEXT("Assign the real MappingContext in the character Blueprint."));
    }
}

void AGPPLightCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (!Input || !MoveAction || !ToggleAction || !DonateAction || !RecoverAction)
    {
        UE_LOG(LogGPPLightExample, Error,
            TEXT("Enhanced Input and all four assigned Input Action assets are required."));
        return;
    }

    Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGPPLightCharacter::Move);
    Input->BindAction(ToggleAction, ETriggerEvent::Started, this, &AGPPLightCharacter::ActivateLight);
    Input->BindAction(DonateAction, ETriggerEvent::Started, this, &AGPPLightCharacter::DonateLight);
    Input->BindAction(RecoverAction, ETriggerEvent::Started, this, &AGPPLightCharacter::RecoverLight);

    // These actions are optional for the legacy T05 four-action configuration.
    if (ZoomAction)
    {
        Input->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AGPPLightCharacter::Zoom);
    }

    if (SprintAction)
    {
        Input->BindAction(SprintAction, ETriggerEvent::Started, this, &AGPPLightCharacter::BeginSprint);
        Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &AGPPLightCharacter::EndSprint);
        Input->BindAction(SprintAction, ETriggerEvent::Canceled, this, &AGPPLightCharacter::EndSprint);
    }
}

void AGPPLightCharacter::Zoom(const FInputActionValue& Value)
{
    const float Axis = Value.Get<float>();
    if (!FMath::IsNearlyZero(Axis))
    {
        const float MinimumLength = FMath::Max(100.0f, CameraMinimumArmLength);
        const float MaximumLength = FMath::Max(MinimumLength, CameraMaximumArmLength);
        DesiredCameraArmLength = FMath::Clamp(
            DesiredCameraArmLength - Axis * CameraZoomStep, MinimumLength, MaximumLength);
    }
}

void AGPPLightCharacter::BeginSprint()
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = FMath::Max(WalkSpeed, SprintSpeed);
    }
}

void AGPPLightCharacter::EndSprint()
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    }
}

void AGPPLightCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D Axis = Value.Get<FVector2D>();
    if (!Controller || Axis.IsNearlyZero())
    {
        return;
    }

    const FRotator Yaw(0.0f, FollowCamera->GetComponentRotation().Yaw, 0.0f);
    AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::X), Axis.Y);
    AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y), Axis.X);
}

void AGPPLightCharacter::ActivateLight()
{
    if (!AbilitySystem || !HasAuthority())
    {
        return;
    }

    const bool bAttempted = AbilitySystem->TryActivateAbilityByClass(UGPPLightAbility::StaticClass(), false);
    UE_LOG(LogGPPLightExample, Display, TEXT("GAS Light activation request: %s"),
        bAttempted ? TEXT("attempted; verify actual light") : TEXT("rejected"));
}

void AGPPLightCharacter::TogglePersonalLight()
{
    if (LightStage == EGPPLightStage::Donated)
    {
        UE_LOG(LogGPPLightExample, Warning, TEXT("Personal Light is donated; activation is denied."));
        return;
    }

    PersonalLight->SetVisibility(!PersonalLight->IsVisible());
}

AGPPLightBeacon* AGPPLightCharacter::FindNearbyBeacon() const
{
    AGPPLightBeacon* Nearest = nullptr;
    float BestDistanceSquared = FMath::Square(250.0f);

    for (TActorIterator<AGPPLightBeacon> It(GetWorld()); It; ++It)
    {
        AGPPLightBeacon* Candidate = *It;
        const float DistanceSquared = FVector::DistSquared(
            GetActorLocation(), Candidate->GetActorLocation());

        if (DistanceSquared <= BestDistanceSquared)
        {
            BestDistanceSquared = DistanceSquared;
            Nearest = Candidate;
        }
    }

    return Nearest;
}

void AGPPLightCharacter::ApplyLightStage(const EGPPLightStage NewStage)
{
    if (LightStage == NewStage && NewStage != EGPPLightStage::Owned)
    {
        return;
    }

    if (AbilitySystem)
    {
        const bool bWasBlocked = AbilitySystem->HasMatchingGameplayTag(GPPLightTags::LightDonated);
        const bool bMustBlock = NewStage == EGPPLightStage::Donated;
        if (bMustBlock && !bWasBlocked)
        {
            AbilitySystem->AddLooseGameplayTag(GPPLightTags::LightDonated);
        }
        else if (!bMustBlock && bWasBlocked)
        {
            AbilitySystem->RemoveLooseGameplayTag(GPPLightTags::LightDonated);
        }
    }

    LightStage = NewStage;
    if (NewStage == EGPPLightStage::Donated)
    {
        PersonalLight->SetVisibility(false);
    }
}

void AGPPLightCharacter::DonateLight()
{
    if (!HasAuthority() || LightStage != EGPPLightStage::Owned)
    {
        return;
    }

    AGPPLightBeacon* Beacon = FindNearbyBeacon();
    if (!Beacon)
    {
        UE_LOG(LogGPPLightExample, Display, TEXT("No beacon within 250 cm; donation rejected."));
        return;
    }

    ApplyLightStage(EGPPLightStage::Donated);
    PoweredBeacon = Beacon;
    Beacon->ApplyPoweredState(true);

    UE_LOG(LogGPPLightExample, Display, TEXT("Light donated once. GAS action blocked; beacon powered."));
}

void AGPPLightCharacter::RecoverLight()
{
    if (!HasAuthority() || LightStage != EGPPLightStage::Donated || !PoweredBeacon.IsValid())
    {
        return;
    }

    // T05 uses the same beacon as a recovery station. T06 will cover distinct
    // pickups, interaction eligibility and anti-softlock progression.
    AGPPLightBeacon* Nearby = FindNearbyBeacon();
    if (Nearby != PoweredBeacon.Get())
    {
        UE_LOG(LogGPPLightExample, Display, TEXT("Return to the powered beacon to recover Light."));
        return;
    }

    ApplyLightStage(EGPPLightStage::Recovered);
    PoweredBeacon->ApplyPoweredState(true);
    UE_LOG(LogGPPLightExample, Display, TEXT("Personal Light recovered. Beacon remains powered."));
}
