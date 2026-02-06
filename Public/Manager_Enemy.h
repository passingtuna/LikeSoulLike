// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Manager_Enemy.generated.h"

/**
 * 
 */
class ACharacterNonPlayableBase;
class ACharacterPlayableBase;
UCLASS()
class LIKESOULLIKE_API UManager_Enemy : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	TWeakObjectPtr<ACharacterPlayableBase> PlayerCharacter;
	TArray<TWeakObjectPtr<ACharacterNonPlayableBase>> arrEnemy;
	void RespawnAll();
	void AddEnemy(TWeakObjectPtr<ACharacterNonPlayableBase>);
	void UpdatePlayerCharacter(TWeakObjectPtr<ACharacterPlayableBase> Pc) { PlayerCharacter = Pc; };
	void GiveEnemySoul(int soul);
};
