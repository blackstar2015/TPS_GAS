// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TPSAbilitySystemGlobals.h"

#include "TPSAbilityTypes.h"


FGameplayEffectContext* UTPSAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FTPSGameplayEffectContext();
}
