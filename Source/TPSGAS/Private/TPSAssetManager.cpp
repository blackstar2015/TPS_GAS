// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "TPSGameplayTags.h"


UTPSAssetManager& UTPSAssetManager::Get()
{
	check(GEngine);
	UTPSAssetManager* TPSAssetManager = Cast<UTPSAssetManager>(GEngine->AssetManager);
	return *TPSAssetManager;
}

void UTPSAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FTPSGameplayTags::InitializeNativeGameplayTags();
	
	//THIS IS REQUIRED TO USE TARGET DATA
	UAbilitySystemGlobals::Get().InitGlobalData();
}
