// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TPSPlayerState.h"

#include <AbilitySystem/TPSAbilitySystemComponent.h>
#include <AbilitySystem/TPSAttributeSet.h>

#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

ATPSPlayerState::ATPSPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UTPSAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UTPSAttributeSet>("AttributeSet");
	SetNetUpdateFrequency(100.f);
}

void ATPSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATPSPlayerState, Level);
	DOREPLIFETIME(ATPSPlayerState, XP);
	DOREPLIFETIME(ATPSPlayerState, AttributePoints);
	DOREPLIFETIME(ATPSPlayerState, SpellPoints);
}

UAbilitySystemComponent* ATPSPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ATPSPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level, true);
}

void ATPSPlayerState::AddToXP(int32 InXP)
{
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void ATPSPlayerState::AddToAttributePoints(int32 InAttributePoints)
{
	AttributePoints += InAttributePoints;
	OnAttributePointChangedDelegate.Broadcast(AttributePoints);
}

void ATPSPlayerState::AddToSpellPoints(int32 InSpellPoints)
{
	SpellPoints += InSpellPoints;
	OnSpellPointChangedDelegate.Broadcast(SpellPoints);
}

void ATPSPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level, false);
}

void ATPSPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void ATPSPlayerState::SetAttributePoints(int32 InAttributePoints)
{
	AttributePoints = InAttributePoints;
	OnAttributePointChangedDelegate.Broadcast(AttributePoints);
}

void ATPSPlayerState::SetSpellPoints(int32 InSpellPoints)
{
	SpellPoints = InSpellPoints;
	OnSpellPointChangedDelegate.Broadcast(SpellPoints);
}

void ATPSPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level, true);
}

void ATPSPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}

void ATPSPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointChangedDelegate.Broadcast(AttributePoints);
}

void ATPSPlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
	OnSpellPointChangedDelegate.Broadcast(SpellPoints);
}
