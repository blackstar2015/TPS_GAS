// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * TPS GameplayTags
 *
 * Singleton containing native Gameplay Tags
 */

struct FTPSGameplayTags
{
public:
	static const FTPSGameplayTags& Get() {return GameplayTags;}
	static void InitializeNativeGameplayTags();

	TMap<FGameplayTag,FGameplayTag> DamageTypesToResistances;
	TMap<FGameplayTag,FGameplayTag> DamageTypesToDebuffs;

#pragma region Primary Tags
	FGameplayTag Attributes_Primary_Vigour;
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Dexterity;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Luck;
#pragma endregion

#pragma region Secondary Tags
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CritChance;
	FGameplayTag Attributes_Secondary_CritDamage;
	FGameplayTag Attributes_Secondary_CritResistance;
	FGameplayTag Attributes_Secondary_HealthRegen;
	FGameplayTag Attributes_Secondary_ManaRegen;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
#pragma endregion

#pragma region Vital Tags
	FGameplayTag Attributes_Vital_Health;
	FGameplayTag Attributes_Vital_Mana;
#pragma endregion
	
#pragma region Meta Attribute Tags
	FGameplayTag Attributes_Meta_IncomingXP;
#pragma endregion

#pragma region Input Tags
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;
#pragma endregion

#pragma region Resistances
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Ice;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;
#pragma endregion

#pragma region Damage Tags
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Ice;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;
#pragma endregion

#pragma region Debuff Tags	
	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Stun;
	FGameplayTag Debuff_Freeze;
	FGameplayTag Debuff_Arcane;
	FGameplayTag Debuff_Physical;
	FGameplayTag Debuff_Chance;
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Duration;
	FGameplayTag Debuff_Frequency;
#pragma endregion

#pragma region Abilities Tags
	FGameplayTag Abilities_None;
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	FGameplayTag Abilities_WaitingExecution;
	FGameplayTag Abilities_HitReact;	
#pragma endregion
		
#pragma region Fire Abilities Tags		
	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Abilities_Fire_FireBlast;
#pragma endregion

#pragma region Lightning Abilities Tags		
	FGameplayTag Abilities_Lightning_Electrocute;
#pragma endregion

#pragma region Arcane Abilities Tags		
	FGameplayTag Abilities_Arcane_ArcaneShards;
#pragma endregion
	
#pragma region Passive Abilities Tags
	FGameplayTag Abilities_Passive_HaloOfProtection;
	FGameplayTag Abilities_Passive_HaloOfProtectionActive;
	FGameplayTag Abilities_Passive_LifeSiphon;
	FGameplayTag Abilities_Passive_LifeSiphonActive;
	FGameplayTag Abilities_Passive_ManaSiphon;
	FGameplayTag Abilities_Passive_ManaSiphonActive;
#pragma endregion

#pragma region Ability Status Tags
	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;
# pragma endregion

#pragma region Ability Types Tags
	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;
#pragma endregion
	
#pragma region Cooldown Tags
	FGameplayTag Cooldown_Fire_FireBolt;
	FGameplayTag Cooldown_Fire_FireBlast;
	FGameplayTag Cooldown_Lightning_Electrocute;
	FGameplayTag Cooldown_Arcane_ArcaneShards;
#pragma endregion
	
#pragma region Combat Socket Tags
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Tail;
#pragma endregion
	
#pragma region Attack Montage tags
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;
#pragma endregion

#pragma region Effect Tags
	FGameplayTag Effects_HitReact;
#pragma endregion

#pragma region Player Tags
    FGameplayTag Player_Block_InputPressed;
    FGameplayTag Player_Block_InputReleased;
    FGameplayTag Player_Block_InputHeld;
    FGameplayTag Player_Block_CursorTrace;
#pragma endregion

#pragma region Gameplay Cue Tags
	FGameplayTag GameplayCue_FireBlast;
#pragma endregion
private:
	static FTPSGameplayTags GameplayTags;
};