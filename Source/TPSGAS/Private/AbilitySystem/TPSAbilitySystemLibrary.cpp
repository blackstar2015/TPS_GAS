// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TPSAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "TPSGameplayTags.h"
#include "Engine/OverlapResult.h"
#include "Game/TPSGameMode.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/TPSPlayerState.h"
#include "UI/HUD/TPSHUD.h"
#include "UI/WidgetControllers/TPSWidgetController.h"

bool UTPSAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject,
                                                          FWidgetControllerParams& OutWCParams, ATPSHUD*& OutTPSHUD)
{
	if(APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		OutTPSHUD = Cast<ATPSHUD>(PC->GetHUD());
		if(OutTPSHUD)
		{
			ATPSPlayerState* PS = PC->GetPlayerState<ATPSPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
		
			OutWCParams.AttributeSet = AS;
			OutWCParams.PlayerController = PC;
			OutWCParams.PlayerState = PS;
			OutWCParams.AbilitySystemComponent = ASC;
			return true;
		}
	}
	return false;
}

UTPSOverlayWidgetController* UTPSAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ATPSHUD* TPSHUD = nullptr;

	if(MakeWidgetControllerParams(WorldContextObject, WCParams, TPSHUD))
	{
		return TPSHUD->GetOverlayWidgetController(WCParams);
	}	
	return nullptr;
}

UAttributeMenuWidgetController* UTPSAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ATPSHUD* TPSHUD = nullptr;
	
	if(MakeWidgetControllerParams(WorldContextObject, WCParams, TPSHUD))
	{
		return TPSHUD->GetAttributeMenuWidgetController(WCParams);		
	}	
	return nullptr;	
}

USpellMenuWidgetController* UTPSAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ATPSHUD* TPSHUD = nullptr;
	
	if(MakeWidgetControllerParams(WorldContextObject, WCParams, TPSHUD))
	{
		return TPSHUD->GetSpellMenuWidgetController(WCParams);		
	}	
	return nullptr;	
}

void UTPSAbilitySystemLibrary::InitializeDefaultAttributes(ECharacterClass CharacterClass, float Level,
                                                           const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultInfo =  CharacterClassInfo->GetClassDefaultInfo( CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);	
	const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle =
		ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes,Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle =
		ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes,Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributeSpecHandle =
		ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes,Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
}

void UTPSAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC,
	ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);		
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UTPSAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const ATPSGameMode* TPSGameMode = Cast<ATPSGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (TPSGameMode == nullptr) return nullptr;
	return TPSGameMode->CharacterClassInfo;
}

UAbilityInfo* UTPSAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const ATPSGameMode* TPSGameMode = Cast<ATPSGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (TPSGameMode == nullptr) return nullptr;
	return TPSGameMode->AbilityInfo;
}

bool UTPSAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTPSGameplayEffectContext* TPSEffectContext =
	static_cast<const FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return TPSEffectContext->IsBlockedHit();
	}
	return false;
}

bool UTPSAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTPSGameplayEffectContext* TPSEffectContext =
	static_cast<const FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return TPSEffectContext->IsCriticalHit();
	}
	return false;
}

bool UTPSAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTPSGameplayEffectContext* TPSEffectContext = static_cast<const FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return TPSEffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float UTPSAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTPSGameplayEffectContext* TPSEffectContext =
	static_cast<const FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return TPSEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float UTPSAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTPSGameplayEffectContext* TPSEffectContext =
	static_cast<const FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return TPSEffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float UTPSAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTPSGameplayEffectContext* TPSEffectContext =
	static_cast<const FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return TPSEffectContext->GetDebuffFrequency();
	}
	return 0;
}

FGameplayTag UTPSAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTPSGameplayEffectContext* TPSEffectContext =
	static_cast<const FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (TPSEffectContext->GetDamageType().IsValid())
		{
			return *TPSEffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

FVector UTPSAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTPSGameplayEffectContext* TPSEffectContext =
	static_cast<const FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{		
		return TPSEffectContext->GetDeathImpulse();		
	}
	return FVector::ZeroVector;
}

FVector UTPSAbilitySystemLibrary::GetKnockBackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTPSGameplayEffectContext* TPSEffectContext =
	static_cast<const FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{		
		return TPSEffectContext->GetKnockBack();		
	}
	return FVector::ZeroVector;
}

bool UTPSAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTPSGameplayEffectContext* TPSEffectContext =
	static_cast<const FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{		
		return TPSEffectContext->IsRadialDamage();		
	}
	return false;
}

float UTPSAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTPSGameplayEffectContext* TPSEffectContext =
	static_cast<const FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{		
		return TPSEffectContext->GetRadialDamageInnerRadius();		
	}
	return 0.f;
}

float UTPSAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTPSGameplayEffectContext* TPSEffectContext =
	static_cast<const FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{		
		return TPSEffectContext->GetRadialDamageOuterRadius();		
	}
	return 0.f;
}

FVector UTPSAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTPSGameplayEffectContext* TPSEffectContext =
	static_cast<const FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{		
		return TPSEffectContext->GetRadialDamageOrigin();		
	}
	return FVector::ZeroVector;
}

void UTPSAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FTPSGameplayEffectContext* TPSEffectContext = static_cast<FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TPSEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UTPSAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsCriticalHit)
{
	if (FTPSGameplayEffectContext* TPSEffectContext = static_cast<FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TPSEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UTPSAbilitySystemLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsSuccessfulDebuff)
{
	if (FTPSGameplayEffectContext* TPSEffectContext = static_cast<FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TPSEffectContext->SetIsSuccessfulDebuff(bInIsSuccessfulDebuff);
	}
}

void UTPSAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDamage)
{
	if (FTPSGameplayEffectContext* TPSEffectContext = static_cast<FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TPSEffectContext->SetDebuffDamage(InDamage);
	}
}

void UTPSAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, float InDuration)
{
	if (FTPSGameplayEffectContext* TPSEffectContext = static_cast<FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TPSEffectContext->SetDebuffDuration(InDuration);
	}
}

void UTPSAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle, float InFrequency)
{
	if (FTPSGameplayEffectContext* TPSEffectContext = static_cast<FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TPSEffectContext->SetDebuffFrequency(InFrequency);
	}
}

void UTPSAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle,
	const FGameplayTag& InDamageType)
{
	if (FTPSGameplayEffectContext* TPSEffectContext = static_cast<FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		TPSEffectContext->SetDamageType(DamageType);
	}
}

void UTPSAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InDeathImpulse)
{
	if (FTPSGameplayEffectContext* TPSEffectContext = static_cast<FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TPSEffectContext->SetDeathImpulse(InDeathImpulse);
	}
}

void UTPSAbilitySystemLibrary::SetKnockBack(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InKnockBack)
{
	if (FTPSGameplayEffectContext* TPSEffectContext = static_cast<FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TPSEffectContext->SetKnockBackForce(InKnockBack);
	}
}

void UTPSAbilitySystemLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsRadialDamage)
{
	if (FTPSGameplayEffectContext* TPSEffectContext = static_cast<FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TPSEffectContext->SetIsRadialDamage(bInIsRadialDamage);
	}
}

void UTPSAbilitySystemLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle,
	float InRadialDamageInnerRadius)
{
	if (FTPSGameplayEffectContext* TPSEffectContext = static_cast<FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TPSEffectContext->SetRadialDamageInnerRadius(InRadialDamageInnerRadius);
	}
}

void UTPSAbilitySystemLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle,
	float InRadialDamageOuterRadius)
{
	if (FTPSGameplayEffectContext* TPSEffectContext = static_cast<FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TPSEffectContext->SetRadialDamageOuterRadius(InRadialDamageOuterRadius);
	}
}

void UTPSAbilitySystemLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InRadialDamageOrigin)
{
	if (FTPSGameplayEffectContext* TPSEffectContext = static_cast<FTPSGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TPSEffectContext->SetRadialDamageOrigin(InRadialDamageOrigin);
	}
}

bool UTPSAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));

	const bool bFriends = bBothArePlayers || bBothAreEnemies;
	return !bFriends;
}

FGameplayEffectContextHandle UTPSAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FTPSGameplayTags& GameplayTags = FTPSGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);

	SetDeathImpulse(EffectContextHandle, DamageEffectParams.DeathImpulse);
	SetKnockBack(EffectContextHandle,DamageEffectParams.KnockBackForce);

	SetIsRadialDamage(EffectContextHandle, DamageEffectParams.bIsRadialDamage);
	SetRadialDamageInnerRadius(EffectContextHandle, DamageEffectParams.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContextHandle, DamageEffectParams.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContextHandle, DamageEffectParams.RadialDamageOrigin);

	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass,DamageEffectParams.AbilityLevel,EffectContextHandle );

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance );
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage );
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration );
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);

	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
}

void UTPSAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
	const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps,SphereOrigin,FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius),SphereParams);

		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(Overlap.GetActor());
			}
		}
	}
}

void UTPSAbilitySystemLibrary::GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors,
	TArray<AActor*>& OutClosestTargets, const FVector& Origin)
{
	if(Actors.Num() <= MaxTargets)
	{
		OutClosestTargets = Actors;
		return;
	}

	TArray<AActor*> ActorsToCheck = Actors;
	int32 NumTargetsFound = 0;

	while (NumTargetsFound < MaxTargets)
	{
		if (ActorsToCheck.Num() == 0) break;
		double ClosestDistance = TNumericLimits<double>::Max();
		AActor* ClosestActor;
		for (AActor* PotentialTarget : ActorsToCheck)
		{
			const double Distance = (PotentialTarget->GetActorLocation() - Origin).Length();
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = PotentialTarget;
			}
		}
		ActorsToCheck.Remove(ClosestActor);
		OutClosestTargets.AddUnique(ClosestActor);
		++NumTargetsFound;
	}
}

TArray<FRotator> UTPSAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis,
	float Spread, int32 NumRotators)
{
	TArray<FRotator> Rotators;	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread/2.f, Axis);

	if (NumRotators > 1)
	{
		const float DeltaSpread = Spread / (NumRotators - 1);
		for (int32 i = 0; i < NumRotators; i++)
		{			
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, Axis);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}
	return Rotators;
}

TArray<FVector> UTPSAbilitySystemLibrary::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis,
	float Spread, int32 NumVectors)
{
	TArray<FVector> Vectors;	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread/2.f, Axis);

	if (NumVectors > 1)
	{
		const float DeltaSpread = Spread / (NumVectors - 1);
		for (int32 i = 0; i < NumVectors; i++)
		{			
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, Axis);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(Forward);
	}
	return Vectors;
}

float UTPSAbilitySystemLibrary::CalculateLaunchAngleDegrees(float Distance, float Speed, float Gravity)
{
	float Argument = (Distance * Gravity) / (Speed * Speed);

	if (Argument > 1.f || Argument < -1.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid input: arcsine(%.2f) is undefined"), Argument);
		return -1.f; // Impossible shot
	}

	float AngleRadians = 0.5f * FMath::Asin(Argument);
	return FMath::RadiansToDegrees(AngleRadians);
}

int32 UTPSAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;

	const FCharacterClassDefaultInfo& Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}

void UTPSAbilitySystemLibrary::SetIsRadialDamageEffectParam(FDamageEffectParams& DamageEffectParams,
	bool bInIsRadialDamage, float InInnerRadius, float InOuterRadius, FVector InOrigin)
{
	DamageEffectParams.bIsRadialDamage = bInIsRadialDamage;
	DamageEffectParams.RadialDamageInnerRadius = InInnerRadius;
	DamageEffectParams.RadialDamageOuterRadius = InOuterRadius;
	DamageEffectParams.RadialDamageOrigin = InOrigin;
}

void UTPSAbilitySystemLibrary::SetKnockbackDirection(FDamageEffectParams& DamageEffectParams,
	FVector KnockbackDirection, float InKnockbackMagnitudeOverride)
{
	KnockbackDirection.Normalize();
	if (InKnockbackMagnitudeOverride == 0.0f) DamageEffectParams.KnockBackForce = KnockbackDirection * DamageEffectParams.KnockBackForceMagnitude;
	else DamageEffectParams.KnockBackForce = KnockbackDirection * InKnockbackMagnitudeOverride;	
}

void UTPSAbilitySystemLibrary::SetDeathImpulseDirection(FDamageEffectParams& DamageEffectParams,
	FVector ImpulseDirection, float InDeathImpulseMagnitudeOverride)
{
	ImpulseDirection.Normalize();
	if (InDeathImpulseMagnitudeOverride == 0.0f) DamageEffectParams.DeathImpulse = ImpulseDirection * DamageEffectParams.DeathImpulseMagnitude;
	else DamageEffectParams.DeathImpulse = ImpulseDirection * InDeathImpulseMagnitudeOverride;
}

void UTPSAbilitySystemLibrary::SetTargetEffectParamsASC(FDamageEffectParams& DamageEffectParams,
	UAbilitySystemComponent* InASC)
{
	DamageEffectParams.TargetAbilitySystemComponent = InASC;
}
