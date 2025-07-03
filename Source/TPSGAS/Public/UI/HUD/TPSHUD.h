// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TPSHUD.generated.h"

class UAttributeMenuWidgetController;
class USpellMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
class UTPSOverlayWidgetController;
class UTPSUserWidget;
struct FWidgetControllerParams;
/**
 * 
 */
UCLASS()
class TPSGAS_API ATPSHUD : public AHUD
{
	GENERATED_BODY()

public:

	UTPSOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);
	USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams);
	void InitOverlay(APlayerController* PC,APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
	UPROPERTY()
	TObjectPtr<UTPSUserWidget> OverlayWidget;
	UPROPERTY(EditAnywhere)  
	TSubclassOf<UTPSUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UTPSOverlayWidgetController> OverlayWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTPSOverlayWidgetController> OverlayWidgetControllerClass;
	
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
	
	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;
};
