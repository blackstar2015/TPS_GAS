// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TPSGameMode.generated.h"

class UCharacterClassInfo;
class UAbilityInfo;
/**
 * 
 */
UCLASS()
class TPSGAS_API ATPSGameMode : public AGameModeBase
{
	GENERATED_BODY()
		
public:
	UPROPERTY(EditDefaultsOnly, Category="Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category="Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	// UPROPERTY(EditDefaultsOnly, Category="Loot Tiers")
	// TObjectPtr<ULootTiers> LootTiers;
	
	// UPROPERTY(EditDefaultsOnly)
	// TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag = FName("PlayerStart");

	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;
	//TODO: Implement saving progress
	// void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);
	// void TravelToMap(UMVVM_LoadSlot* LoadSlot);
	// void SaveInGameProgressData(ULoadScreenSaveGame* SaveObject);
	// void SaveWorldState(UWorld* World, const FString& DestinationMapAssetName = FString("")) const;
	// void LoadWorldState(UWorld* World) const;
	// static void DeleteSlot(const FString& SlotName, int32 SlotIndex);
	// ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;
	// ULoadScreenSaveGame* RetrieveInGameSaveData();	
	// FString GetMapNameFromMapAssetName(const FString& MapAssetName) const;
	// virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	//
	void PlayerDied(ACharacter*  DeadCharacter);
protected:
	virtual void BeginPlay() override;
};
