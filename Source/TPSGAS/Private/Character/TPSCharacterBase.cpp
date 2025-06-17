
#include "Character/TPSCharacterBase.h"

ATPSCharacterBase::ATPSCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATPSCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}
