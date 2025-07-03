// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/TPSPlayerCharacter.h"
#include "TPSGameplayTags.h"
#include "AbilitySystem/TPSAbilitySystemComponent.h"
//#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/TPSPlayerController.h"
#include "Player/TPSPlayerState.h"
#include "UI/HUD/TPSHUD.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/TPSAbilitySystemLibrary.h"
#include "AbilitySystem/TPSAttributeSet.h"
//#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "Game/TPSGameMode.h"
//#include "Game/LoadScreenSaveGame.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

ATPSPlayerCharacter::ATPSPlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ATPSPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();
}

void ATPSPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
}

int32 ATPSPlayerCharacter::GetPlayerLevel_Implementation()
{
	const ATPSPlayerState* TPSPlayerState = GetPlayerState<ATPSPlayerState>();
	check(TPSPlayerState);
	return TPSPlayerState->GetPlayerLevel();
}

void ATPSPlayerCharacter::Die(const FVector& DeathImpulse)
{
	Super::Die(DeathImpulse);

	FTimerDelegate DeathTimerDelegate;
	DeathTimerDelegate.BindLambda([this]()
	{
		ATPSGameMode* TPSGameMode = Cast<ATPSGameMode>(UGameplayStatics::GetGameMode(this));
		if (TPSGameMode)
		{
			TPSGameMode->PlayerDied(this);
		}
	});
	GetWorldTimerManager().SetTimer(DeathTimer, DeathTimerDelegate, DeathTime, false);
	FollowCamera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

void ATPSPlayerCharacter::AddToXP_Implementation(int32 InXP)
{
	IPlayerInterface::AddToXP_Implementation(InXP);
}

void ATPSPlayerCharacter::LevelUp_Implementation()
{
	IPlayerInterface::LevelUp_Implementation();
}

void ATPSPlayerCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	IPlayerInterface::AddToPlayerLevel_Implementation(InPlayerLevel);
}

void ATPSPlayerCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	IPlayerInterface::AddToAttributePoints_Implementation(InAttributePoints);
}

void ATPSPlayerCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	IPlayerInterface::AddToSpellPoints_Implementation(InSpellPoints);
}

int32 ATPSPlayerCharacter::GetXP_Implementation() const
{
	return IPlayerInterface::GetXP_Implementation();
}

int32 ATPSPlayerCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	return IPlayerInterface::FindLevelForXP_Implementation(InXP);
}

int32 ATPSPlayerCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	return IPlayerInterface::GetAttributePointsReward_Implementation(Level);
}

int32 ATPSPlayerCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	return IPlayerInterface::GetSpellPointsReward_Implementation(Level);
}

int32 ATPSPlayerCharacter::GetAttributePoints_Implementation() const
{
	return IPlayerInterface::GetAttributePoints_Implementation();
}

int32 ATPSPlayerCharacter::GetSpellPoints_Implementation() const
{
	return IPlayerInterface::GetSpellPoints_Implementation();
}

void ATPSPlayerCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	IPlayerInterface::ShowMagicCircle_Implementation(DecalMaterial);
}

void ATPSPlayerCharacter::HideMagicCircle_Implementation()
{
	IPlayerInterface::HideMagicCircle_Implementation();
}

void ATPSPlayerCharacter::SaveProgress_Implementation(const FName& CheckpointTag)
{
	IPlayerInterface::SaveProgress_Implementation(CheckpointTag);
}

void ATPSPlayerCharacter::InitAbilityActorInfo()
{
	//Init Ability Actor info for the server
	ATPSPlayerState* TPSPlayerState = GetPlayerState<ATPSPlayerState>();
	check(TPSPlayerState);
	 TPSPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(TPSPlayerState,this);
	 Cast<UTPSAbilitySystemComponent>(TPSPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	 AbilitySystemComponent = TPSPlayerState->GetAbilitySystemComponent();
	 AttributeSet = TPSPlayerState->GetAttributeSet();
	 OnASCRegistered.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FTPSGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ATPSPlayerCharacter::StunTagChanged);
	if(ATPSPlayerController* TPSPlayerController = Cast<ATPSPlayerController>(GetController()))
	{
		if(ATPSHUD* TPSHUD = Cast<ATPSHUD>(TPSPlayerController->GetHUD()))
		{
			TPSHUD->InitOverlay(TPSPlayerController,TPSPlayerState,AbilitySystemComponent,AttributeSet);
		}
	}
}

void ATPSPlayerCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = FollowCamera->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();

		const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();

		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);		
		LevelUpNiagaraComponent->Activate(true);
	}
}
