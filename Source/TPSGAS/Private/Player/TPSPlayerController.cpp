// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TPSPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "TPSGameplayTags.h"
#include "Character/TPSPlayerCharacter.h"
#include "Components/DecalComponent.h"
#include "Input/TPSInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "UI/Widgets/DamageTextComponent.h"
class UEnhancedInputLocalPlayerSubsystem;


ATPSPlayerController::ATPSPlayerController()
{
	bReplicates = true;
}

void ATPSPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CameraTrace();
}

void ATPSPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if(!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
		if(DecalMaterial)
		{
			MagicCircle->MagicCircleDecal->SetMaterial(0,DecalMaterial);
		}
	}
}

void ATPSPlayerController::HideMagicCircle()
{
	if(IsValid(MagicCircle))
	{
		MagicCircle->Destroy();
	}
}

void ATPSPlayerController::ShowDamageNumber_Implementation(float Damage, ACharacter* TargetCharacter, bool bBlockedHit,
                                                           bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter,DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);		
		DamageText->SetDamageText(Damage,bBlockedHit,bCriticalHit);
	}
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

	UTPSInputComponent* TPSInputComponent = CastChecked<UTPSInputComponent>(InputComponent);
	
	// Jumping
	TPSInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATPSPlayerController::Jump);
	TPSInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ATPSPlayerController::StopJumping);

	// Moving
	TPSInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPSPlayerController::Move);

	// Looking
	TPSInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATPSPlayerController::Look);

	TPSInputComponent->BindAbilityActions(InputConfig,this,
		&ThisClass::AbilityInputTagPressed,&ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void ATPSPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	// if (GetASC() && GetASC()->HasMatchingGameplayTag(FTPSGameplayTags::Get().Player_Block_InputPressed))
	// {
	// 	return;
	// }
	// if (InputTag.MatchesTagExact(FTPSGameplayTags::Get().InputTag_LMB))
	// {
	// 	if (IsValid(ThisActor.GetObject()))
	// 	{
	// 		TargetingStatus = ThisActor.GetObject()->Implements<UEnemyInterface>() ? ETargetingStatus::TargetingEnemy : ETargetingStatus::TargetingNonEnemy;
	// 	}
	// 	else
	// 	{
	// 		TargetingStatus = ETargetingStatus::NotTargeting;
	// 	}
	// 	//bAutoRunning = false;			
	// }	
	// if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
	GEngine->AddOnScreenDebugMessage(1,3.f,FColor::Red,*InputTag.ToString());
}

void ATPSPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	// if (GetASC() && GetASC()->HasMatchingGameplayTag(FTPSGameplayTags::Get().Player_Block_InputReleased))
	// {
	// 	return;
	// }
	// if (!InputTag.MatchesTagExact(FTPSGameplayTags::Get().InputTag_LMB))
	// {
	// 	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
	// 	return;
	// }
	//
	// if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);

	GEngine->AddOnScreenDebugMessage(2,3.f,FColor::Blue,*InputTag.ToString());

#pragma region AutoRunning in Top Down System(DEPRECATED)
	// if (TargetingStatus != ETargetingStatus::TargetingEnemy && !bShiftKeyDown)
	// {
	// 	const APawn* ControlledPawn = GetPawn();
	// 	if (FollowTime <= ShortPressThreshold && ControlledPawn)
	// 	{
	// 		if (IsValid(ThisActor) && ThisActor->Implements<UHighlightInterface>())
	// 		{
	// 			IHighlightInterface::Execute_SetMoveToLocation(ThisActor,CachedDestination);
	// 		}
	// 		else if (GetASC() && !GetASC()->HasMatchingGameplayTag(FTPSGameplayTags::Get().Player_Block_InputPressed))
	// 		{
	// 			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
	// 		}
	// 		
	// 		if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
	// 		{
	// 			Spline->ClearSplinePoints();
	// 			for (const FVector& PointLoc : NavPath->PathPoints)
	// 			{
	// 				Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
	// 			}
	// 			if (NavPath->PathPoints.Num() > 0)
	// 			{
	// 				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
	// 				bAutoRunning = true;
	// 			}
	// 		}			
	// 	}
	// 	FollowTime = 0.f;
	// 	TargetingStatus = ETargetingStatus::NotTargeting;
	// }
#pragma endregion
}

void ATPSPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// if (GetASC() && GetASC()->HasMatchingGameplayTag(FTPSGameplayTags::Get().Player_Block_InputHeld))
	// {
	// 	return;
	// }
	//
	// 	if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	//
	// if (TargetingStatus == ETargetingStatus::TargetingEnemy)
	// {
	// 	if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	// }
	GEngine->AddOnScreenDebugMessage(3,3.f,FColor::Green,*InputTag.ToString());

#pragma region AutoRunning in Top Down System(DEPRECATED)
	// else
	// {
	// 	FollowTime += GetWorld()->GetDeltaSeconds();
	// 	if (CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint;
	//
	// 	if (APawn* ControlledPawn = GetPawn())
	// 	{
	// 		const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
	// 		ControlledPawn->AddMovementInput(WorldDirection);
	// 	}
	// }
#pragma endregion
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

UTPSAbilitySystemComponent* ATPSPlayerController::GetASC()
{
	if(TPSAbilitySystemComponent == nullptr)
	{
		TPSAbilitySystemComponent = Cast<UTPSAbilitySystemComponent>
		(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return TPSAbilitySystemComponent;
}
