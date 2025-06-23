// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "TPSEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TPSGAS_API ATPSEnemyCharacter : public ATPSCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	ATPSEnemyCharacter();
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
};
