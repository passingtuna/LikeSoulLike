// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LikeSoulLikeType.h"
#include "Manager_Inventory.generated.h"

/**
 * 
 */
UCLASS()
class LIKESOULLIKE_API UManager_Inventory : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	TArray<FItemData> Inventory;
	TArray<FItemData> Storage;

	FItemData QuickSlot[3];
	FItemData WeaponSlot[3];

	void AddItemToInventory();
	void MoveInventoryToStorage(FItemData InventItem,FItemData Storage);

	void RegistQuickSlot(int slot, FItemData itemdata);
	void RegistWeaponSlot(int slot, FItemData itemdata);
};
