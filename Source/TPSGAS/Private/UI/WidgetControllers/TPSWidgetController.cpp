// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetControllers/TPSWidgetController.h"

#include "Player/TPSPlayerController.h"
#include "Player/TPSPlayerState.h"
#include "AbilitySystem/TPSAbilitySystemComponent.h"
#include "AbilitySystem/TPSAttributeSet.h"


void UTPSWidgetController::SetWidgetControllerParams(const FWidgetControllerParams WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UTPSWidgetController::BroadCastInitialValues()
{
	
}

void UTPSWidgetController::BindCallbacksToDependencies()
{
	
}


 void UTPSWidgetController::BroadcastAbilityInfo()
	{
		if (!GetTPSASC()->bStartupAbilitiesGiven) return;
		
		FForEachAbility BroadcastDelegate;
		BroadcastDelegate.BindLambda(
			[this](const FGameplayAbilitySpec& AbilitySpec)
			{
				FTPSAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(
					TPSAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
				Info.StatusTag = TPSAbilitySystemComponent->GetStatusFromSpec(AbilitySpec);
				Info.InputTag = TPSAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
				AbilityInfoDelegate.Broadcast(Info);
			});
		GetTPSASC()->ForEachAbility(BroadcastDelegate);
	}

ATPSPlayerController* UTPSWidgetController::GetTPSPlayerController()
{
	if (TPSPlayerController == nullptr )
	{
		TPSPlayerController = Cast<ATPSPlayerController>(PlayerController);
	}
	return TPSPlayerController;
}

ATPSPlayerState* UTPSWidgetController::GetTPSPlayerState()
{
	if (TPSPlayerState == nullptr )
	{
		TPSPlayerState = Cast<ATPSPlayerState>(PlayerState);
	}
	return TPSPlayerState;
}

UTPSAbilitySystemComponent* UTPSWidgetController::GetTPSASC()
{
	if (TPSAbilitySystemComponent == nullptr )
	{
		TPSAbilitySystemComponent = Cast<UTPSAbilitySystemComponent>(AbilitySystemComponent);
	}
	return TPSAbilitySystemComponent;
}

UTPSAttributeSet* UTPSWidgetController::GetTPSAttributeSet()
{
	if ((TPSAttributeSet == nullptr))
	{
		TPSAttributeSet = Cast<UTPSAttributeSet>(AttributeSet);
	}
	return TPSAttributeSet;
}

