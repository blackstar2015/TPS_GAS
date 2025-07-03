// Copyright by Pradeep Pillai


#include "Actors/TPSFireBall.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TPSGameplayTags.h"
#include "GameplayCueManager.h"
#include "AbilitySystem/TPSAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"

void ATPSFireBall::OnSphereOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
									   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;
	
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;
						
			DamageEffectParams.TargetAbilitySystemComponent = TargetAsc;
			UTPSAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
	}
}

void ATPSFireBall::BeginPlay()
{
	Super::BeginPlay();
	StartOutgoingTimeline();
}

void ATPSFireBall::OnHit()
{
	if(GetOwner())
	{
		FGameplayCueParameters CueParameters;
		CueParameters.Location = GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(),FTPSGameplayTags::Get().GameplayCue_FireBlast, CueParameters);
	}
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}


	bHit = true;
}
