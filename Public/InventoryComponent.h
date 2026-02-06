// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LikeSoulLikeType.h"
#include "InventoryComponent.generated.h"

#define STORAGE_MAX_STACK_COUNT 99

class UManager_ItemInfo;
class ACharacterPlayableBase;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIKESOULLIKE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UManager_ItemInfo* ItemInfoManager;
	ACharacterPlayableBase* OwnerCharacter;
	TArray<FItemData>Inventory;
	TArray<FItemData>Storage;

	FItemData* WeaponQuickSlot[3]; 
	FItemData* ItemQuickSlot[3];
	void SetInventoryOwner(ACharacterPlayableBase* character) { OwnerCharacter = character; };

	void EquipWeapon(int32 slotindex, int32 invenIndex);
	void EquipQuickSlot(int32 slotindex, int32 invenIndex);
	bool ConsumeItem(FItemData * data, int32 Count);
	void MoveStorageToInventory(FItemData& data);
	void MoveInventoryToStorage(FItemData& data);

	int32 PutInItemToInventory(const FItemData & data);
	int32 PutInItemToStrage(const FItemData & ItemData );
	int32 TryAddToContainer(TArray<FItemData>& Container, const FItemData& InOutData, int32 MaxStack);

	FItemData GetWeaponSlotWeapon(int32 slot);
	FItemData GetQuickSlotItem(int32 slot);

	FItemData* GetQuickSlotItemInfo(int32 slot);
	FItemData* GetWeaponSlotWeaponInfo(int32 slot);

	void RefillInventory();
};
