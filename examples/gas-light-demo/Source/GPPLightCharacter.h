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
    virtual void PawnClientRestart() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    void TogglePersonalLight();

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

private:
    UPROPERTY(VisibleInstanceOnly, Category = "Demo|Progress")
    EGPPLightStage LightStage = EGPPLightStage::Owned;

    TWeakObjectPtr<AGPPLightBeacon> PoweredBeacon;

    void Move(const FInputActionValue& Value);
    void ActivateLight();
    void DonateLight();
    void RecoverLight();
    AGPPLightBeacon* FindNearbyBeacon() const;
    void ApplyLightStage(EGPPLightStage NewStage);
};
