// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPSGASGameMode.h"
#include "TPSGASCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATPSGASGameMode::ATPSGASGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
