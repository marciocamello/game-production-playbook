#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GPPLightAbility.generated.h"

// Activating this granted GAS ability toggles a real PointLight on the owning character.
UCLASS()
class UGPPLightAbility final : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UGPPLightAbility();

protected:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;
};
