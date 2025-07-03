
#include "Character/TPSCharacterBase.h"

#include "TPSGameplayTags.h"
#include "AbilitySystem/TPSAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ATPSCharacterBase::ATPSCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATPSCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//EffectAttachComponent->SetWorldRotation(FRotator::ZeroRotator);
}

void ATPSCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPSCharacterBase, bIsStunned);
	DOREPLIFETIME(ATPSCharacterBase, bIsBurned);
	DOREPLIFETIME(ATPSCharacterBase, bIsBeingShocked);
}

float ATPSCharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageTaken = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	OnDamageDelegate.Broadcast(DamageTaken);
	return DamageTaken;
}

UAbilitySystemComponent* ATPSCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ATPSCharacterBase::Die(const FVector& DeathImpulse)
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath(DeathImpulse);
}

FVector ATPSCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	const FTPSGameplayTags& GameplayTags = FTPSGameplayTags::Get();
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);		
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);		
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);		
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Tail))
	{
		return GetMesh()->GetSocketLocation(TailSocketName);		
	}
	return FVector();
}

bool ATPSCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* ATPSCharacterBase::GetAvatar_Implementation()
{
	return this;
}

UAnimMontage* ATPSCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

TArray<FTaggedMontage> ATPSCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

UNiagaraSystem* ATPSCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

FTaggedMontage ATPSCharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (FTaggedMontage TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag == MontageTag)
		{
			return  TaggedMontage;
		}
	}
	return FTaggedMontage();
}

int32 ATPSCharacterBase::GetMinionCount_Implementation()
{
	return MinionCount;
}

void ATPSCharacterBase::IncrementMinionCount_Implementation(int32 Amount)
{
	MinionCount += Amount;
	return;
}

ECharacterClass ATPSCharacterBase::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

FOnASCRegistered& ATPSCharacterBase::GetOnASCRegisteredDelegate()
{
	return OnASCRegistered;
}

FOnDeathSignature& ATPSCharacterBase::GetOnDeathDelegate()
{
	return OnDeathDelegate;
}

USkeletalMeshComponent* ATPSCharacterBase::GetWeapon_Implementation()
{
	return Weapon;
}

void ATPSCharacterBase::SetIsBeingShocked_Implementation(bool bInShock)
{
	bIsBeingShocked = bInShock;
}

bool ATPSCharacterBase::IsBeingShocked_Implementation() const
{
	return bIsBeingShocked;
}

FOnDamageSignature& ATPSCharacterBase::GetOnDamageDelegate()
{
	return OnDamageDelegate;
}

void ATPSCharacterBase::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	UGameplayStatics::PlaySoundAtLocation(this,DeathSound,GetActorLocation(),GetActorRotation());
	
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Weapon->AddImpulse(DeathImpulse * .1f, NAME_None,true);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Block);
	GetMesh()->AddImpulse(DeathImpulse,NAME_None,true);
	
	Dissolve();
	DissolveWeapon();
	bDead = true;
	
	//TODO: Handle Debuff components
	//BurnDebuffComponent->Deactivate();
	//StunDebuffComponent->Deactivate();
	
	OnDeathDelegate.Broadcast(this);
}

void ATPSCharacterBase::OnRep_Stunned()
{
}

void ATPSCharacterBase::OnRep_Burned()
{
}

void ATPSCharacterBase::OnRep_BeingShocked()
{
}

void ATPSCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle =  GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass,Level,ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),GetAbilitySystemComponent());
}

void ATPSCharacterBase::AddCharacterAbilities()
{
	UTPSAbilitySystemComponent* TPSASC = CastChecked<UTPSAbilitySystemComponent>(AbilitySystemComponent);
	if(!HasAuthority()) return;

	TPSASC->AddCharacterAbilities(StartupAbilities);
	TPSASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

void ATPSCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance,this);
		GetMesh()->SetMaterial(0, DynamicMatInst);

		StartDissolveTimeline(DynamicMatInst);
	}
}

void ATPSCharacterBase::DissolveWeapon()
{
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance,this);
		Weapon->SetMaterial(0, DynamicMatInst);

		StartWeaponDissolveTimeline(DynamicMatInst);
	}
}

void ATPSCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATPSCharacterBase::InitAbilityActorInfo()
{
	
}

void ATPSCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes,1);
	ApplyEffectToSelf(DefaultSecondaryAttributes,1);
	ApplyEffectToSelf(DefaultVitalAttributes,1);
}

void ATPSCharacterBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsStunned = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned? 0.f: BaseWalkSpeed;
}
