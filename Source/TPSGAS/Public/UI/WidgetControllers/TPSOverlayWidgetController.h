// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetControllers/TPSWidgetController.h"
#include "TPSOverlayWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class TPSGAS_API UTPSOverlayWidgetController : public UTPSWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadCastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	
};
