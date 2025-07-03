// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSAbilityTypes.h"
#include "AbilitySystem/Abilities/TPSGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "TPSDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class TPSGAS_API UTPSDamageGameplayAbility : public UTPSGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(
	AActor* TargetActor = nullptr,
	FVector InRadialDamageOrigin = FVector::ZeroVector,
	bool bOverrideKnockbackDirection = false,
	FVector KnockbackDirectionOverride = FVector::ZeroVector,
	bool bOverrideDeathImpulse = false,
	FVector DeathImpulseDirectionOverride = FVector::ZeroVector,
	bool bOverridePitch = false,
	float PitchOverride = 0.f) const;

	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel() const;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage")
	FScalableFloat BaseDamage;

	//TODO: You can make these scalable floats
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage|Debuff")
	float DebuffChance = 20.f;

	//You can randomize these and apply level curves to these as well
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage|Debuff")
	float DebuffDamage = 5.f;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage|Debuff")
	float DebuffDuration = 5.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage|Debuff")
	float DebuffFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage|Damage Forces")
	float DeathImpulseMagnitude = 1000.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage|Damage Forces")
	float KnockBackForceMagnitude = 1000.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage|Damage Forces")
	float KnockBackChance = 0.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage|Radial")
	bool bIsRadialDamage = false;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage|Radial")
	float RadialDamageInnerRadius = 0.f;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage|Radial")
	float RadialDamageOuterRadius = 0.f;
	
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;
};
