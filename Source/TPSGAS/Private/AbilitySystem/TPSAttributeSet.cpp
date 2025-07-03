// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TPSAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "TPSGameplayTags.h"
#include "AbilitySystem/TPSAbilitySystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Player/TPSPlayerController.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"



UTPSAttributeSet::UTPSAttributeSet()
{
	
	const FTPSGameplayTags& GameplayTags = FTPSGameplayTags::Get();

#pragma region Primary Attributes
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength,GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence,GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Dexterity,GetDexterityAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigour,GetVigourAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Luck,GetLuckAttribute);
#pragma endregion

#pragma region Secondary Attributes
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor,GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration,GetArmorPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance,GetBlockChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CritChance,GetCritChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CritDamage,GetCritDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CritResistance,GetCritResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegen,GetHealthRegenAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegen,GetManaRegenAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth,GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana,GetMaxManaAttribute);
#pragma endregion
	
#pragma region  Resistance Attributes
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Fire,GetFireResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Lightning,GetLightningResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Ice,GetIceResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Arcane,GetArcaneResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Physical,GetPhysicalResistanceAttribute);
#pragma endregion
}

void UTPSAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
#pragma region Primary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet,Strength,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet,Intelligence,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet,Dexterity,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet,Vigour,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet,Luck,COND_None,REPNOTIFY_Always);
#pragma endregion
	
#pragma region  Secondary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet,Armor,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet,ArmorPenetration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet,BlockChance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet,CritChance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet,CritDamage,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet,CritResistance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet,HealthRegen,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet,ManaRegen,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
#pragma endregion

#pragma region Resistance Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet,FireResistance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet,LightningResistance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet,IceResistance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet,ArcaneResistance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet,PhysicalResistance,COND_None,REPNOTIFY_Always);
#pragma endregion	

#pragma region Vital Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSAttributeSet,Mana,COND_None,REPNOTIFY_Always);
#pragma endregion
}

void UTPSAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	if(Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
}

void UTPSAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if(Props.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Props.TargetCharacter)) return;
	
	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(),0.f,GetMaxHealth()));
	}
	if(Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(),0.f,GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props);
	}
	if(Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP(Props);
	}
}

void UTPSAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	if (Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOffHealth = false;
	}
	if (Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana = false;
	}
}

void UTPSAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
}

void UTPSAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
}

void UTPSAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const
{
}

void UTPSAttributeSet::OnRep_Luck(const FGameplayAttributeData& OldLuck) const
{
}

void UTPSAttributeSet::OnRep_Vigour(const FGameplayAttributeData& OldVigour) const
{
}

void UTPSAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
}

void UTPSAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const
{
}

void UTPSAttributeSet::OnRep_IceResistance(const FGameplayAttributeData& OldIceResistance) const
{
}

void UTPSAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const
{
}

void UTPSAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
}

void UTPSAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldStrength) const
{
}

void UTPSAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
}

void UTPSAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
}

void UTPSAttributeSet::OnRep_CritChance(const FGameplayAttributeData& OldCritChance) const
{
}

void UTPSAttributeSet::OnRep_CritDamage(const FGameplayAttributeData& OldCritDamage) const
{
}

void UTPSAttributeSet::OnRep_CritResistance(const FGameplayAttributeData& OldCritResistance) const
{
}

void UTPSAttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const
{
}

void UTPSAttributeSet::OnRep_ManaRegen(const FGameplayAttributeData& OldManaRegen) const
{
}

void UTPSAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPSAttributeSet, Health, OldHealth);
}

void UTPSAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPSAttributeSet, MaxHealth, OldMaxHealth);
}

void UTPSAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPSAttributeSet, Mana, OldMana);
}

void UTPSAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	if(LocalIncomingDamage > 0)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth,0.f,GetMaxHealth()));
		
		const bool bFatal = NewHealth <= 0.f;
		if (bFatal)
		{
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor))
			{
				//Death Impulse
				const FVector DeathImpulse = UTPSAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle);
				CombatInterface->Die(DeathImpulse);
			}
			SendXPEvent(Props);
		}
		else
		{
			if (Props.TargetCharacter->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsBeingShocked(Props.TargetCharacter))
			{				
				//HitReact
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FTPSGameplayTags::Get().Effects_HitReact);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}
			
			const FVector KnockBackForce = UTPSAbilitySystemLibrary::GetKnockBackForce(Props.EffectContextHandle);
			if(!KnockBackForce.IsNearlyZero(1.f))
			{
				//KnockBack
				const FVector KnockBack = UTPSAbilitySystemLibrary::GetKnockBackForce(Props.EffectContextHandle);
				Props.TargetCharacter->GetCharacterMovement()->StopMovementImmediately();
				Props.TargetCharacter->LaunchCharacter(KnockBack,true,false);
			}
			
		}
		const bool bBlocked = UTPSAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
		const bool bCritical = UTPSAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
			
		ShowFloatingText(Props, LocalIncomingDamage,bBlocked,bCritical);
		if (UTPSAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle))
		{
			//stuff
			Debuff(Props);
		}
// 					
// #pragma region LifeSiphon
// 		FGameplayTagContainer LifeTagContainer;
// 		LifeTagContainer.AddTag(FTPSGameplayTags::Get().Abilities_Passive_LifeSiphon);
// 		if (Props.SourceASC->HasAnyMatchingGameplayTags(LifeTagContainer))
// 		{
// 			SendLifeSiphonEvent(Props);
// 		}
// #pragma endregion
//
// #pragma region ManaSiphon
// 		FGameplayTagContainer ManaTagContainer;
// 		ManaTagContainer.AddTag(FTPSGameplayTags::Get().Abilities_Passive_ManaSiphon);
// 		if (Props.SourceASC->HasAnyMatchingGameplayTags(ManaTagContainer))
// 		{
// 			SendManaSiphonEvent(Props);
// 		}
// #pragma endregion
	}
}

void UTPSAttributeSet::HandleIncomingXP(const FEffectProperties& Props)
{
}

void UTPSAttributeSet::Debuff(const FEffectProperties& Props)
{
	const FTPSGameplayTags& GameplayTags = FTPSGameplayTags::Get();
	FGameplayEffectContextHandle EffectContext = Props.SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(Props.SourceAvatarActor);

	const FGameplayTag DamageType = UTPSAbilitySystemLibrary::GetDamageType(Props.EffectContextHandle);
	const float DebuffDamage = UTPSAbilitySystemLibrary::GetDebuffDamage(Props.EffectContextHandle);
	const float DebuffDuration = UTPSAbilitySystemLibrary::GetDebuffDuration(Props.EffectContextHandle);
	const float DebuffFrequency = UTPSAbilitySystemLibrary::GetDebuffFrequency(Props.EffectContextHandle);
	
	FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageType.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));

	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = DebuffFrequency;
	Effect->DurationMagnitude = FScalableFloat(DebuffDuration);

	FInheritedTagContainer TagContainer = FInheritedTagContainer();
	UTargetTagsGameplayEffectComponent& Component = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	const FGameplayTag DebuffTag = GameplayTags.DamageTypesToDebuffs[DamageType];
	TagContainer.Added.AddTag(DebuffTag);
	//Apply Block input tags if stunned debuff
	if (DebuffTag.MatchesTagExact(GameplayTags.Debuff_Stun))
	{
		TagContainer.Added.AddTag(GameplayTags.Player_Block_CursorTrace);
		TagContainer.Added.AddTag(GameplayTags.Player_Block_InputHeld);
		TagContainer.Added.AddTag(GameplayTags.Player_Block_InputPressed);
		TagContainer.Added.AddTag(GameplayTags.Player_Block_InputReleased);
	}
	Component.SetAndApplyTargetTagChanges(TagContainer);

	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;

	const int32 Index = Effect->Modifiers.Num();
	Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];

	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = UTPSAttributeSet::GetIncomingDamageAttribute();
	
	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f))
	{
		FTPSGameplayEffectContext* TPSContext = static_cast<FTPSGameplayEffectContext*>(MutableSpec->GetContext().Get());
		TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable(new FGameplayTag(DamageType));
		TPSContext->SetDamageType(DebuffDamageType);
		
		Props.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
	}
}

void UTPSAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	Props.EffectContextHandle =  Data.EffectSpec.GetContext();
	
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if(IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if(Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if(const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if(Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void UTPSAttributeSet::ShowFloatingText(const FEffectProperties Props, float Damage, bool bBlockedHit,
	bool bCriticalHit) const
{
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		if (ATPSPlayerController* PlayerController = Cast<ATPSPlayerController>(Props.SourceCharacter->Controller))
		{
			PlayerController->ShowDamageNumber(Damage, Props.TargetCharacter,bBlockedHit,bCriticalHit);
			return;
		}
		if (ATPSPlayerController* PlayerController = Cast<ATPSPlayerController>(Props.TargetCharacter->Controller))
		{
			PlayerController->ShowDamageNumber(Damage, Props.TargetCharacter,bBlockedHit,bCriticalHit);
		}
	}
}

void UTPSAttributeSet::SendXPEvent(const FEffectProperties& Props) const
{
}

void UTPSAttributeSet::SendLifeSiphonEvent(const FEffectProperties& Props) const
{
}

void UTPSAttributeSet::SendManaSiphonEvent(const FEffectProperties& Props) const
{
}

void UTPSAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPSAttributeSet, MaxMana, OldMaxMana);
}
