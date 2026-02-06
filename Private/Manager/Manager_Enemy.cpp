// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager_Enemy.h"
#include "CharacterNonPlayableBase.h"
#include "CharacterPlayableBase.h"

void UManager_Enemy::RespawnAll()
{
    for (int32 i = arrEnemy.Num() - 1; i >= 0; --i)
    {
        if (!arrEnemy[i].IsValid())
        {
            arrEnemy.RemoveAtSwap(i);
            continue;
        }
        arrEnemy[i]->Respawn();
    }
}

void UManager_Enemy::AddEnemy(TWeakObjectPtr<ACharacterNonPlayableBase> enemy)
{
    arrEnemy.Add(enemy);
}
void UManager_Enemy::GiveEnemySoul(int soul) 
{
    if (PlayerCharacter.IsValid())
    {
        PlayerCharacter->ModifyCurrentSoul(soul);
    }
}