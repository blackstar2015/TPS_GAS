// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/TPSGameMode.h"

void ATPSGameMode::PlayerDied(ACharacter* DeadCharacter)
{
	
}

void ATPSGameMode::BeginPlay()
{
	Super::BeginPlay();
	Maps.Add(DefaultMapName, DefaultMap);
}
