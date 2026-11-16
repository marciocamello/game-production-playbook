#include "GPPLightAbility.h"

#include "GPPLightCharacter.h"
#include "GPPLightTags.h"

UGPPLightAbility::UGPPLightAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;

    // GAS checks this tag before ability activation; it is not merely a UI indicator.
    ActivationBlockedTags.AddTag(GPPLightTags::LightDonated);
}

void UGPPLightAbility::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    AGPPLightCharacter* Character = Cast<AGPPLightCharacter>(GetAvatarActorFromActorInfo());
    if (!Character || !CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    Character->TogglePersonalLight();
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
