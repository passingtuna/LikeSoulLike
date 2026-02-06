// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Manager_ItemInfo.h"
#include "PDA_ItemDetailInfo.h"
#include "CharacterPlayableBase.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	ItemInfoManager = GetWorld()->GetGameInstance()->GetSubsystem<UManager_ItemInfo>();
	// ...
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::EquipWeapon(int32 slotindex, int32 invenindex)
{
	if (slotindex < 3 && invenindex < Inventory.Num())
	{
		for (int i = 0 ; i <  3 ; ++i)
		{
			if (WeaponQuickSlot[i] == &Inventory[invenindex])
			{
				Inventory[invenindex].IsEquiped = false;
				WeaponQuickSlot[i] = nullptr;
				OwnerCharacter->ResetWeaponSlotInfo(i);
			}
		}
		if(WeaponQuickSlot[slotindex]) WeaponQuickSlot[slotindex]->IsEquiped = false;
		WeaponQuickSlot[slotindex] = &Inventory[invenindex];
		WeaponQuickSlot[slotindex]->IsEquiped = true;
		if (WeaponQuickSlot[slotindex])
		{
			FItemPresentationData temp = ItemInfoManager->GetItemPresentationData(WeaponQuickSlot[slotindex]->ID, (OwnerCharacter->CurrentWeaponSlotNum == slotindex));
			OwnerCharacter->AddWeaponToMainSlot(temp.Bp_Weapon, slotindex);
			
		}
	}
}
void UInventoryComponent::EquipQuickSlot(int32 slotindex, int32 invenindex)
{
	if (invenindex < Inventory.Num() && slotindex < 3)
	{
		for (auto & Elem : ItemQuickSlot)
		{
			if (Elem == &Inventory[invenindex])
			{
				Inventory[invenindex].IsEquiped = false;
				Elem = nullptr;
			}
		}
		if (ItemQuickSlot[slotindex]) ItemQuickSlot[slotindex]->IsEquiped = false;
		ItemQuickSlot[slotindex] = &Inventory[invenindex];
		ItemQuickSlot[slotindex]->IsEquiped = true;
	}
}

int32 UInventoryComponent::PutInItemToInventory(const FItemData& data)
{
	FItemData Copydata = data;
	const FItemBaseData ItemInstantInfo = ItemInfoManager->GetItemBaseData(Copydata.ID);
	int32 result = TryAddToContainer(Inventory, Copydata, ItemInstantInfo.MaxStackCount);

	Copydata.Count = result;
	if (result > 0)
	{
		return PutInItemToStrage(Copydata);
	}
	return result;
}
int32 UInventoryComponent::PutInItemToStrage(const FItemData &data)
{
	EItemMoveRestriction moveRestriction = static_cast<EItemMoveRestriction>(ItemInfoManager->GetItemBaseData(data.ID).MoveRestriction);

	if (EnumHasAnyFlags(moveRestriction , EItemMoveRestriction::NoStorage) || data.IsEquiped) return data.Count;

	if (data.ID == NAME_None) return data.Count;

	return TryAddToContainer(Storage,data,STORAGE_MAX_STACK_COUNT);
}

int32 UInventoryComponent::TryAddToContainer(TArray<FItemData>& Container, const FItemData& data, int32 MaxStack)
{
	int32 result;
	const FName TargetID = data.ID;
	if (TargetID == NAME_None) return data.Count;
	int32 Index = Container.IndexOfByPredicate( [TargetID](const FItemData& Item) 
		{ 
			return Item.ID == TargetID;
		}
	);
	
	if (Index == INDEX_NONE || MaxStack == 1)
	{
		if (data.Count > MaxStack)
		{
			FItemData NewData = data;
			NewData.Count = MaxStack;
			result = data.Count - MaxStack;
			Container.Add(NewData);
			return result;
		}
		else
		{
			Container.Add(data);
			result = 0;
			return result;
		}
	}
	else
	{
		int32 calResult = Container[Index].Count + data.Count;

		if (calResult > MaxStack)
		{
			result = calResult - MaxStack;
			Container[Index].Count = MaxStack;
			return result;
		}
		else
		{
			Container[Index].Count = calResult;
			result = 0;
			return result;
		}
	}
}
bool UInventoryComponent::ConsumeItem(FItemData * data, int32 Count)
{
	if (!data) return false;
	int32 resultCount = (*data).Count - Count;
	if (resultCount < 0) return false;// 사용못하는데 왜들어왔니
	(*data).Count = resultCount;
	return true;
}

void UInventoryComponent::MoveInventoryToStorage(FItemData& InventoryItemdata)
{
	const FName TargetID = InventoryItemdata.ID;
	const FItemBaseData ItemBaseInfo = ItemInfoManager->GetItemBaseData(InventoryItemdata.ID);
	int32 MaxCount = ItemBaseInfo.MaxStackCount;

	EItemMoveRestriction moveRestriction = static_cast<EItemMoveRestriction>(ItemInfoManager->GetItemBaseData(InventoryItemdata.ID).MoveRestriction);

	if (EnumHasAnyFlags(moveRestriction, EItemMoveRestriction::NoStorage) || InventoryItemdata.IsEquiped) return;

	int32 Index = Storage.IndexOfByPredicate([TargetID](const FItemData& Item)
		{
			return Item.ID == TargetID;
		});


	if (Index == INDEX_NONE || MaxCount == 1)
	{
		Storage.Add(InventoryItemdata);
		InventoryItemdata.Count = 0;
	}
	else if (Storage.IsValidIndex(Index))
	{
		int32 insufficientCount = STORAGE_MAX_STACK_COUNT - Storage[Index].Count;

		if (InventoryItemdata.Count > insufficientCount)
		{
			InventoryItemdata.Count -= insufficientCount;
			Storage[Index].Count = STORAGE_MAX_STACK_COUNT;
		}
		else
		{
			Storage[Index].Count += InventoryItemdata.Count;
			InventoryItemdata.Count = 0;
		}
	}
}

void UInventoryComponent::MoveStorageToInventory(FItemData& Itemdat)
{
	const FName TargetID = Itemdat.ID;
	const FItemBaseData ItemBaseInfo = ItemInfoManager->GetItemBaseData(Itemdat.ID);
	int32 MaxCount = ItemBaseInfo.MaxStackCount;
	int32 StorageIndex = Storage.IndexOfByPredicate([TargetID](const FItemData& Item)
		{
			return Item.ID == TargetID;
		});
	if (!Storage.IsValidIndex(StorageIndex))return;

	int32 InventoryIndex = Inventory.IndexOfByPredicate([TargetID](const FItemData& Item)
		{
			return Item.ID == TargetID;
		});

	UE_LOG(LogTemp, Warning, TEXT("2"));
	if (Inventory[InventoryIndex].Count >= MaxCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("3"));
		Inventory[InventoryIndex].Count = MaxCount;
		return;
	}

	if (InventoryIndex == INDEX_NONE || MaxCount == 1)
	{
		Inventory.Add(Storage[StorageIndex]);
		Storage[StorageIndex].Count = 0;
		UE_LOG(LogTemp, Warning, TEXT("3"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("4-1  : %d"), MaxCount);
		UE_LOG(LogTemp, Warning, TEXT("4-2  : %d"), Inventory[InventoryIndex].Count);
		int32 insufficientCount = MaxCount - Inventory[InventoryIndex].Count;

		UE_LOG(LogTemp, Warning, TEXT("4  : %d"), insufficientCount);
		if (Storage[StorageIndex].Count > insufficientCount)
		{
			UE_LOG(LogTemp, Warning, TEXT("5 %d"), Storage[StorageIndex].Count);
			Storage[StorageIndex].Count -= insufficientCount;
			Inventory[InventoryIndex].Count = MaxCount;
			UE_LOG(LogTemp, Warning, TEXT("6 %d"), Storage[StorageIndex].Count);
		}
		else
		{
			Inventory[InventoryIndex].Count += Storage[StorageIndex].Count;
			Storage[StorageIndex].Count = 0;
			UE_LOG(LogTemp, Warning, TEXT("7"));
		}
	}
}


FItemData UInventoryComponent::GetWeaponSlotWeapon(int32 slot)
{ 
	FItemData temp;
	if (slot < 3 && WeaponQuickSlot[slot])
	{
		return *WeaponQuickSlot[slot];
	}
	return temp;
}

FItemData UInventoryComponent::GetQuickSlotItem(int32 slot)
{
	FItemData temp;
	if (slot < 3 && ItemQuickSlot[slot])
	{
		return *ItemQuickSlot[slot];
	}
	return temp;
}

FItemData* UInventoryComponent::GetQuickSlotItemInfo(int32 slot)
{
	if (slot < 3 && ItemQuickSlot[slot]) return ItemQuickSlot[slot];
	return nullptr;
}

FItemData* UInventoryComponent::GetWeaponSlotWeaponInfo(int32 slot)
{
	if (slot < 3 && WeaponQuickSlot[slot]) return WeaponQuickSlot[slot];
	return nullptr;
}




void UInventoryComponent::RefillInventory()
{
	for (auto& Elem : Inventory)
	{
		FItemBaseData baseData= ItemInfoManager->GetItemBaseData(Elem.ID);
		
		switch (baseData.ItemType)
		{
		case EItemType::IT_Refillable:
			{
				Elem.Count = baseData.MaxStackCount;
			}
			break;
		case EItemType::IT_Consumable:
			{
				MoveStorageToInventory(Elem);
			}
			break;
		default:
			continue;
			break;
		}

	}
}