// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetControllers/AttributeMenuWidgetController.h"

#include "AbilitySystem/TPSAbilitySystemComponent.h"
#include "AbilitySystem/TPSAttributeSet.h"
#include "Player/TPSPlayerState.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	check(AttributeInfo)	
	for (auto& Pair : GetTPSAttributeSet()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
		[this,Pair](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(Pair.Key, Pair.Value());
		}
	);
	}

	GetTPSPlayerState()->OnAttributePointChangedDelegate.AddLambda(
		[this](int32 AttributePoints)
		{
			AttributePointsChangeDelegate.Broadcast(AttributePoints);
		}
	);
	
}

void UAttributeMenuWidgetController::BroadCastInitialValues()
{
	UTPSAttributeSet* AS = CastChecked<UTPSAttributeSet>(AttributeSet);
	check(AttributeInfo)
	for (auto& Pair : GetTPSAttributeSet()->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
	AttributePointsChangeDelegate.Broadcast(GetTPSPlayerState()->GetAttributePoints());
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UTPSAbilitySystemComponent* TPSASC = CastChecked<UTPSAbilitySystemComponent>(AbilitySystemComponent);

	TPSASC->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	FTPSAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
