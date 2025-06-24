
#include "Character/TPSCharacterBase.h"

ATPSCharacterBase::ATPSCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* ATPSCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ATPSCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATPSCharacterBase::InitAbilityActorInfo()
{
	
}
