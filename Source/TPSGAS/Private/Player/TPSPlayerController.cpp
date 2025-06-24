// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TPSPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Character/TPSPlayerCharacter.h"
#include "Interaction/EnemyInterface.h"

class UEnhancedInputLocalPlayerSubsystem;

ATPSPlayerController::ATPSPlayerController()
{
	bReplicates = true;
}

void ATPSPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CameraTrace();
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

void ATPSPlayerController::CameraTrace()
{
	FVector WorldLocation, WorldDirection;
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	FVector2D ScreenCenter(ViewportSizeX / 2.f, ViewportSizeY / 2.f);

	if (!DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldLocation, WorldDirection))
	{
		return;
	}

	FVector TraceStart = WorldLocation;
	FVector TraceEnd = TraceStart + (WorldDirection * 1500.f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetPawn());

	TArray<FHitResult> HitResults;
	const ECollisionChannel TraceChannel = ECC_Visibility;

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		TraceStart,
		TraceEnd,
		FQuat::Identity,
		TraceChannel,
		FCollisionShape::MakeSphere(TraceRadius),
		Params
	);

	TScriptInterface<IEnemyInterface> BestTarget = nullptr;
	float BestAngle = FLT_MAX;

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		if (!IsValid(HitActor)) continue;

		if (HitActor->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
		{
			FVector ToTarget = (HitActor->GetActorLocation() - WorldLocation).GetSafeNormal();
			float Angle = FMath::Acos(FVector::DotProduct(WorldDirection, ToTarget));

			if (Angle < BestAngle)
			{
				BestAngle = Angle;
				BestTarget = TScriptInterface<IEnemyInterface>(HitActor);
			}
		}
	}

	LastActor = ThisActor;
	ThisActor = BestTarget;

	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->HighlightActor();
	}

	CurrentTarget = Cast<AActor>(ThisActor.GetObject());// Optional separate AActor* pointer

	if (IsValid(CurrentTarget))
	{
		//DrawDebugLine(GetWorld(), WorldLocation, CurrentTarget->GetActorLocation(), FColor::Green, false, .1f, 0, 1.0f);
		//DrawDebugLine(GetWorld(), WorldLocation, TraceEnd, FColor::Blue, false, .1f, 0, 1.0f);
	}
	else
	{
		//DrawDebugLine(GetWorld(), WorldLocation, TraceEnd, FColor::Red, false, .1f, 0, 1.0f);
	}
}
