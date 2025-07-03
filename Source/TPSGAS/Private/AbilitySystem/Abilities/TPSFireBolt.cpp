// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TPSFireBolt.h"

#include "AbilitySystem/TPSAbilitySystemLibrary.h"
#include "Actors/TPSProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

FString UTPSFireBolt::GetDescription(int32 Level)
{
	const int32 Damage = BaseDamage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	if(Level == 1)
	{
		return 	FString::Printf(TEXT(
			"<Title> Fire Bolt</>"
			"\n\n<Small>Current Level: </><Level>%d</>"
			"\n<Small>Mana Cost: </><ManaCost>%.1f</>"
			"\n<Small>Cooldown: </><Cooldown>%.1f</>"
			"\n\n<Default>Launches a bolt of fire,exploding on impact and dealing: "
			"</><Damage>%d</><Default> fire damage with a chance to burn.</>"
			),
			 Level,
			 ManaCost,
			 Cooldown,
			 Damage
			 );
	}
	else
	{		
		return 	FString::Printf(TEXT(
			"<Title> Fire Bolt</>"
			"\n\n<Small>Current Level: </><Level>%d</>"
			"\n<Small>Mana Cost: </><ManaCost>%.1f</>"
			"\n<Small>Cooldown: </><Cooldown>%.1f</>"
			"\n\n<Default>Launches %d bolts of fire,exploding on impact and dealing: "
			"</><Damage>%d</><Default> fire damage with a chance to burn.</>"
			),
			 Level,
			 ManaCost,
			 Cooldown,
			 FMath::Min(Level,NumProjectiles),
			 Damage);
	}
}

FString UTPSFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 Damage = BaseDamage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		"<Title> Next Level:</>"
		"\n\n<Small>Next Level: </><Level>%d</>"
		"\n<Small>Mana Cost: </><ManaCost>%.1f</>"
		"\n<Small>Cooldown: </><Cooldown>%.1f</>"
		"\n\n<Default>Launches %d bolts of fire,exploding on impact and dealing: "
		"</><Damage>%d</><Default> fire damage with a chance to burn.</>"
		),
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level,NumProjectiles),
		Damage);
}

void UTPSFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
	bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag);
	
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	const FVector Forward = Rotation.Vector();
	
	if(bUseAbilityLevelForNumProjectiles) NumProjectiles = FMath::Min(GetAbilityLevel(), MaxNumProjectiles);
	TArray<FRotator> Rotations = UTPSAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, NumProjectiles);
	
	for (const FRotator& Rotator : Rotations)
	{
		float HomingAccelerationMag = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		FVector TargetLocation = HomingTarget->GetActorLocation();
		const float DistanceToTarget = FVector::Dist(SocketLocation, TargetLocation);
		PitchOverride = UTPSAbilitySystemLibrary::CalculateLaunchAngleDegrees(DistanceToTarget, HomingAccelerationMag);
		if (bOverridePitch) Rotation.Pitch = PitchOverride;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotator.Quaternion());
	
		ATPSProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATPSProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		
		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{
			Projectile-> HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}
		
		Projectile->ProjectileMovement->HomingAccelerationMagnitude = HomingAccelerationMag;
		Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::Min(HomingAccelerationMin, HomingAccelerationMax);
		Projectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}
