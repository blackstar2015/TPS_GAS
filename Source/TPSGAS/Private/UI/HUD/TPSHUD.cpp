// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/TPSHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/WidgetControllers/TPSOverlayWidgetController.h"
#include "UI/Widgets/TPSUserWidget.h"

UTPSOverlayWidgetController* ATPSHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if(OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UTPSOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}
//
// UAttributeMenuWidgetController* ATPSHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
// {
// 	if(AttributeMenuWidgetController == nullptr)
// 	{
// 		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
// 		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams); 
// 		AttributeMenuWidgetController->BindCallbacksToDependencies();
// 	}
// 	return AttributeMenuWidgetController;
// }
//
// USpellMenuWidgetController* ATPSHUD::GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams)
// {
// 	if(SpellMenuWidgetController == nullptr)
// 	{
// 		SpellMenuWidgetController = NewObject<USpellMenuWidgetController>(this, SpellMenuWidgetControllerClass);
// 		SpellMenuWidgetController->SetWidgetControllerParams(WCParams);
// 		SpellMenuWidgetController->BindCallbacksToDependencies();
// 	}
// 	return SpellMenuWidgetController;
// }

void ATPSHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay widget class uninitialized. Please fill out BP_TPSHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay widget controller class uninitialized. Please fill out BP_TPSHUD"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(),OverlayWidgetClass);
	OverlayWidget = Cast<UTPSUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC,PS ,ASC,AS );
	UTPSOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);
	
	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadCastInitialValues();
	
	Widget->AddToViewport();
}

