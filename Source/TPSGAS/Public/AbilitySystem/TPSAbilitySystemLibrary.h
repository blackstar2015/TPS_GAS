// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSAbilityTypes.h"
#include "GameplayEffectTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/AbilityInfo.h"
//#include "Data/CharacterClassInfo.h"
#include "TPSAbilitySystemLibrary.generated.h"

class ATPSHUD;
struct FWidgetControllerParams;
//class USpellMenuWidgetController;
class UAbilitySystemComponent;
//class UAttributeMenuWidgetController;
//class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class TPSGAS_API UTPSAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
#pragma region Widget Controller
	// UFUNCTION(BlueprintPure,Category="TPSAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	// static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, ATPSHUD*& OutTPSHUD);
	
	// UFUNCTION(BlueprintPure,Category="TPSAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	// static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	//
	// UFUNCTION(BlueprintPure,Category="TPSAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	// static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	//
	// UFUNCTION(BlueprintPure,Category="TPSAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	// static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);	
#pragma endregion

#pragma region Ability System Class Defaults
	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|CharacterClassDefaults")
	// static void InitializeDefaultAttributes(ECharacterClass CharacterClass, float Level, const UObject* WorldContextObject, UAbilitySystemComponent* ASC);

	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|CharacterClassDefaults")
	// static void InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ULoadScreenSaveGame* SaveGame);
	
	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|CharacterClassDefaults")
	// static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|CharacterClassDefaults")
	// static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|CharacterClassDefaults")
	// static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|CharacterClassDefaults", meta = (DefaultToSelf = "WorldContextObject"))
	// static ULootTiers* GetLootTiers(const UObject* WorldContextObject);
#pragma endregion

#pragma region Effect Context Getters
	// UFUNCTION(BlueprintPure,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	//
	// UFUNCTION(BlueprintPure,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	//
	// UFUNCTION(BlueprintPure,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);
	//
	// UFUNCTION(BlueprintPure,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	//
	// UFUNCTION(BlueprintPure,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);
	//
	// UFUNCTION(BlueprintPure,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);
	//
	// UFUNCTION(BlueprintPure,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);
	//
	// UFUNCTION(BlueprintPure,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle);
	//
	// UFUNCTION(BlueprintPure,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static FVector GetKnockBackForce(const FGameplayEffectContextHandle& EffectContextHandle);
	//
	// UFUNCTION(BlueprintPure,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static bool IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	//
	// UFUNCTION(BlueprintPure,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static float GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle);
	//
	// UFUNCTION(BlueprintPure,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static float GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle);
	//
	// UFUNCTION(BlueprintPure,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static FVector GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle);	
#pragma endregion

#pragma region Effect Context Setters
	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);
	//
	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);
	//
	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static void SetIsSuccessfulDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsSuccessfulDebuff);
	//
	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDamage);
	//
	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDuration);
	//
	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InFrequency);
	//
	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);
	//
	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,const FVector& InDeathImpulse);
	//
	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static void SetKnockBack(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,const FVector& InKnockBack);
	//
	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static void SetIsRadialDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,bool bInIsRadialDamage);
	//
	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static void SetRadialDamageInnerRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,float InRadialDamageInnerRadius);
	//
	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static void SetRadialDamageOuterRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,float InRadialDamageOuterRadius);
	//
	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|GameplayEffects")
	// static void SetRadialDamageOrigin(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,const FVector& InRadialDamageOrigin);
#pragma endregion

#pragma region Gameplay Mechanics
	// UFUNCTION(BlueprintPure,Category="TPSAbilitySystemLibrary|GameplayMechanics")
	// static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);
	
	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|Damage Effects")
	// static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);
	
	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|GameplayMechanics")
	// static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,
	// 	const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);
	//
	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|GameplayMechanics")
	// static void GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin);
	//
	// UFUNCTION(BlueprintPure,Category="TPSAbilitySystemLibrary|GameplayMechanics")
	// static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators);
	//
	// UFUNCTION(BlueprintPure,Category="TPSAbilitySystemLibrary|GameplayMechanics")
	// static TArray<FVector> EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors);
	//
	// UFUNCTION(BlueprintPure,Category="TPSAbilitySystemLibrary|GameplayMechanics")
	// static  float CalculateLaunchAngleDegrees(float Distance, float Speed, float Gravity = 980.f);
	
	//static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);
#pragma endregion

#pragma region DamageEffectParams
	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|DamageEffects")
	// static void SetIsRadialDamageEffectParam(UPARAM(ref) FDamageEffectParams& DamageEffectParams, bool bInIsRadialDamage, float InInnerRadius, float InOuterRadius, FVector InOrigin);
	//
	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|DamageEffects")
	// static void SetKnockbackDirection(UPARAM(ref) FDamageEffectParams& DamageEffectParams,FVector KnockbackDirection, float InKnockbackMagnitudeOverride = 0.f);
	//
	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|DamageEffects")
	// static void SetDeathImpulseDirection(UPARAM(ref) FDamageEffectParams& DamageEffectParams,FVector ImpulseDirection, float InDeathImpulseMagnitudeOverride = 0.f);
	//
	// UFUNCTION(BlueprintCallable,Category="TPSAbilitySystemLibrary|DamageEffects")
	// static void SetTargetEffectParamsASC(UPARAM(ref) FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InASC);
#pragma endregion
};
