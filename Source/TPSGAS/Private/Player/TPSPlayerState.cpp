// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TPSPlayerState.h"

#include <AbilitySystem/TPSAbilitySystemComponent.h>
#include <AbilitySystem/TPSAttributeSet.h>

#include "AbilitySystemComponent.h"

ATPSPlayerState::ATPSPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UTPSAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UTPSAttributeSet>("AttributeSet");
	
	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* ATPSPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}