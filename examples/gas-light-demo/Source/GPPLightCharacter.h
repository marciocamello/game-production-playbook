#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GPPLightCharacter.generated.h"

class AGPPLightBeacon;
class UAbilitySystemComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UPointLightComponent;
class USpringArmComponent;
class UStaticMeshComponent;
struct FInputActionValue;
class UInputComponent;

// Single source of truth for this one-map, one-player learning exercise.
UENUM(BlueprintType)
enum class EGPPLightStage : uint8
{
    Owned,
    Donated,
    Recovered
};

UCLASS()
class AGPPLightCharacter final : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    AGPPLightCharacter();

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void PawnClientRestart() override;
    virtual void UnPossessed() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    void TogglePersonalLight();

    // Camera collision is observable independently of requested zoom.
    UFUNCTION(BlueprintPure, Category = "Demo|Camera")
    bool IsCameraRetracted() const;

    UFUNCTION(BlueprintPure, Category = "Demo|Camera")
    float GetCameraRetractionCm() const;

    // Call when a pause/menu/cinematic takes input focus without a Sprint release.
    UFUNCTION(BlueprintCallable, Category = "Demo|Input")
    void ResetSprint();

    UFUNCTION(BlueprintPure, Category = "Demo|Gameplay")
    EGPPLightStage GetLightStage() const { return LightStage; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Demo|GAS")
    TObjectPtr<UAbilitySystemComponent> AbilitySystem;

    UPROPERTY(VisibleAnywhere, Category = "Demo|Visual")
    TObjectPtr<UStaticMeshComponent> CharacterMarker;

    UPROPERTY(VisibleAnywhere, Category = "Demo|Visual")
    TObjectPtr<UPointLightComponent> PersonalLight;

    UPROPERTY(VisibleAnywhere, Category = "Demo|Camera")
    TObjectPtr<USpringArmComponent> CameraArm;

    UPROPERTY(VisibleAnywhere, Category = "Demo|Camera")
    TObjectPtr<UCameraComponent> FollowCamera;

    // Real Input Action and Mapping Context assets are authored in Unreal Editor.
    UPROPERTY(EditDefaultsOnly, Category = "Demo|Input")
    TObjectPtr<UInputMappingContext> MappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "Demo|Input")
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditDefaultsOnly, Category = "Demo|Input")
    TObjectPtr<UInputAction> ToggleAction;

    UPROPERTY(EditDefaultsOnly, Category = "Demo|Input")
    TObjectPtr<UInputAction> DonateAction;

    UPROPERTY(EditDefaultsOnly, Category = "Demo|Input")
    TObjectPtr<UInputAction> RecoverAction;

    // Optional extra actions introduced by T08; T05's four-action demo still runs.
    UPROPERTY(EditDefaultsOnly, Category = "Demo|Input")
    TObjectPtr<UInputAction> ZoomAction;

    UPROPERTY(EditDefaultsOnly, Category = "Demo|Input")
    TObjectPtr<UInputAction> SprintAction;

    UPROPERTY(EditDefaultsOnly, Category = "Demo|Camera", meta = (ClampMin = "100.0"))
    float CameraMinimumArmLength = 380.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Demo|Camera", meta = (ClampMin = "100.0"))
    float CameraMaximumArmLength = 800.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Demo|Camera", meta = (ClampMin = "1.0"))
    float CameraZoomStep = 95.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Demo|Camera", meta = (ClampMin = "0.1"))
    float CameraZoomInterpolation = 10.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Demo|Movement", meta = (ClampMin = "1.0"))
    float WalkSpeed = 380.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Demo|Movement", meta = (ClampMin = "1.0"))
    float SprintSpeed = 570.0f;

private:
    UPROPERTY(VisibleInstanceOnly, Category = "Demo|Progress")
    EGPPLightStage LightStage = EGPPLightStage::Owned;

    TWeakObjectPtr<AGPPLightBeacon> PoweredBeacon;
    float DesiredCameraArmLength = 560.0f;
    bool bWasCameraRetracted = false;

    void Zoom(const FInputActionValue& Value);
    void BeginSprint();
    void EndSprint();
    void Move(const FInputActionValue& Value);
    void ActivateLight();
    void DonateLight();
    void RecoverLight();
    AGPPLightBeacon* FindNearbyBeacon() const;
    void ApplyLightStage(EGPPLightStage NewStage);
};
