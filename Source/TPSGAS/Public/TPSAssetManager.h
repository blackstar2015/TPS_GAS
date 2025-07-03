// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "TPSAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class TPSGAS_API UTPSAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:

	static UTPSAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;

};
