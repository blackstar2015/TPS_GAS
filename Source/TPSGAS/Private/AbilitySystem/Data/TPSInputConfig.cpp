// Copyright by Pradeep Pillai


#include "AbilitySystem/Data/TPSInputConfig.h"

#include "GameplayTagContainer.h"
#include "InputAction.h"

const UInputAction* UTPSInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for(const FTPSInputAction& Action : AbilityInputActions)
	{
		if(Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
		if(bLogNotFound)
		{
			UE_LOG(LogTemp, Error, TEXT("Cant Find Ability Input Action for Input Tag [%s], on inputConfig [%s]"),
				*InputTag.ToString(), *GetNameSafe(this));
		}
	}
	return nullptr;
}
