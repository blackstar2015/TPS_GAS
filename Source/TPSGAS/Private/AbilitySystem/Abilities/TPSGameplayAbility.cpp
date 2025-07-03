// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TPSGameplayAbility.h"

#include "AbilitySystem/TPSAttributeSet.h"

FString UTPSGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Name - LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum", Level);
}

FString UTPSGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n<Default>Causes much more damage. </>"), Level);
}

FString UTPSGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell Locked Until Level: %d</>"), Level);
}

float UTPSGameplayAbility::GetManaCost(float InLevel) const
{
	float ManaCost = 0.0f;
	if(const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for(FGameplayModifierInfo Mod : CostEffect->Modifiers)
		{
			if(Mod.Attribute == UTPSAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

float UTPSGameplayAbility::GetCooldown(float InLevel) const
{
	float Cooldown = 0.0f;
	if(const UGameplayEffect* CoolDownEffect = GetCooldownGameplayEffect())
	{
		CoolDownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
	}
	return Cooldown;
}
