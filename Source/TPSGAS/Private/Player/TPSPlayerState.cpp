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

void ATPSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

UAbilitySystemComponent* ATPSPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ATPSPlayerState::AddToLevel(int32 InLevel)
{
}

void ATPSPlayerState::AddToXP(int32 InXP)
{
}

void ATPSPlayerState::AddToAttributePoints(int32 InAttributePoints)
{
}

void ATPSPlayerState::AddToSpellPoints(int32 InSpellPoints)
{
}

void ATPSPlayerState::SetLevel(int32 InLevel)
{
}

void ATPSPlayerState::SetXP(int32 InXP)
{
}

void ATPSPlayerState::SetAttributePoints(int32 InAttributePoints)
{
}

void ATPSPlayerState::SetSpellPoints(int32 InSpellPoints)
{
}

void ATPSPlayerState::OnRep_Level(int32 OldLevel)
{
}

void ATPSPlayerState::OnRep_XP(int32 OldXP)
{
}

void ATPSPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
}

void ATPSPlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
}
