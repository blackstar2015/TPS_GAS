// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TPSPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Character/TPSPlayerCharacter.h"

class UEnhancedInputLocalPlayerSubsystem;

ATPSPlayerController::ATPSPlayerController()
{
	bReplicates = true;
}

void ATPSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(MappingContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);

	Subsystem->AddMappingContext(MappingContext,0);
	
	bShowMouseCursor = false;
	DefaultMouseCursor = EMouseCursor::Default;

	// FInputModeGameAndUI InputModeData;
	// FInputModeGameOnly GameOnlyInputData;
	// InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	// InputModeData.SetHideCursorDuringCapture(true);
	// SetInputMode(InputModeData);
}

void ATPSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	// Jumping
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATPSPlayerController::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ATPSPlayerController::StopJumping);

	// Moving
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPSPlayerController::Move);

	// Looking
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATPSPlayerController::Look);
}

void ATPSPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = 	GetControlRotation();
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(	APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void ATPSPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// add yaw and pitch input to controller
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATPSPlayerController::Jump(const FInputActionValue& InputActionValue)
{
	if (ACharacter* PlayerCharacter = GetCharacter())
	{
		PlayerCharacter->Jump();
	}
}

void ATPSPlayerController::StopJumping(const FInputActionValue& InputActionValue)
{
	if (ACharacter* PlayerCharacter = GetCharacter())
	{
		PlayerCharacter->StopJumping();
	}
}

