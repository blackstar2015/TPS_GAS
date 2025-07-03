// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TPSProjectileSpell.h"
#include "TPSFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class TPSGAS_API UTPSFireBolt : public UTPSProjectileSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,bool bOverridePitch = false, float PitchOverride = 0.f, AActor* HomingTarget = nullptr);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float ProjectileSpread = 90.f;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	int32 MaxNumProjectiles = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float HomingAccelerationMin = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float HomingAccelerationMax = 3200.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	bool bLaunchHomingProjectiles = true;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	bool bUseAbilityLevelForNumProjectiles = true;
};
