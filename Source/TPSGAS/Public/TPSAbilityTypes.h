// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectTypes.h"
#include "TPSAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams(){}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent = nullptr;

	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float KnockBackForceMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float KnockBackChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	FVector KnockBackForce = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	bool bIsRadialDamage = false;

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageInnerRadius = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float RadialDamageOuterRadius = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	FVector RadialDamageOrigin = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FTPSGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsCriticalHit() const {return bIsCriticalHit;}
	bool IsBlockedHit() const {return bIsBlockedHit;}
	bool IsSuccessfulDebuff() const {return bIsSuccessfulDebuff;}
	bool IsRadialDamage() const {return bIsRadialDamage;}
	float GetDebuffDamage() const {return DebuffDamage;}
	float GetDebuffDuration() const {return DebuffDuration;}
	float GetDebuffFrequency() const {return DebuffFrequency;}
	float GetRadialDamageInnerRadius() const {return RadialDamageInnerRadius;}
	float GetRadialDamageOuterRadius() const {return RadialDamageOuterRadius;}
	FVector GetRadialDamageOrigin() const {return RadialDamageOrigin;}
	FVector GetDeathImpulse() const {return  DeathImpulse;}
	FVector GetKnockBack() const {return  KnockBackForce;}
	TSharedPtr<FGameplayTag> GetDamageType() const {return DamageType;}

	void SetIsCriticalHit(const bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit;}
	void SetIsBlockedHit(const bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit;}
	void SetIsSuccessfulDebuff(const bool bInIsSuccessfulDebuff) {bIsSuccessfulDebuff = bInIsSuccessfulDebuff;}
	void SetIsRadialDamage(const bool bInIsRadialDamage) {bIsRadialDamage = bInIsRadialDamage;}
	void SetRadialDamageInnerRadius(const float InRadialDamageInnerRadius) {RadialDamageInnerRadius = InRadialDamageInnerRadius;}
	void SetRadialDamageOuterRadius(const float InRadialDamageOuterRadius) {RadialDamageOuterRadius = InRadialDamageOuterRadius;}
	void SetRadialDamageOrigin(const FVector& InRadialDamageOrigin) {RadialDamageOrigin = InRadialDamageOrigin;}
	void SetDeathImpulse(const FVector& InImpulse) {DeathImpulse = InImpulse;}
	void SetKnockBackForce(const FVector& InKnockBack) {KnockBackForce = InKnockBack;}
	void SetDebuffDamage(const float InDebuffDamage) {DebuffDamage = InDebuffDamage;}
	void SetDebuffDuration(const float InDebuffDuration) {DebuffDuration = InDebuffDuration;}
	void SetDebuffFrequency(const float InDebuffFrequency) {DebuffFrequency = InDebuffFrequency;}
	void SetDamageType(const TSharedPtr<FGameplayTag>& InDamageType) {DamageType = InDamageType;}
	
	virtual UScriptStruct* GetScriptStruct() const 
	{
		return StaticStruct();
	}
	
	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FTPSGameplayEffectContext* Duplicate() const
	{
		FTPSGameplayEffectContext* NewContext = new FTPSGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
			
protected:
	UPROPERTY()
	bool bIsBlockedHit = false;
	
	UPROPERTY()
	bool bIsCriticalHit = false;
	
	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;
	
	UPROPERTY()
	float DebuffFrequency = 0.f;
	
	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY()
	FVector KnockBackForce = FVector::ZeroVector;

	UPROPERTY()
	bool bIsRadialDamage = false;

	UPROPERTY()
	float RadialDamageInnerRadius = 0.f;
	
	UPROPERTY()
	float RadialDamageOuterRadius = 0.f;
	
	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector;
	
	TSharedPtr<FGameplayTag> DamageType;	
};

template<>
struct TStructOpsTypeTraits<FTPSGameplayEffectContext> : TStructOpsTypeTraitsBase2<FTPSGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};