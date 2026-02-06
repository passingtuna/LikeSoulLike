// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_Storage.h"
#include "InventoryComponent.h"
#include "Manager_ItemInfo.h"
#include "UW_ItemObject.h"
#include "UW_ItemContainer.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "CharacterPlayableBase.h"

void UUW_Storage::InitStorage(ACharacterPlayableBase* Character)
{
    InventoryComp = Character->GetInventoryComponent();
    PlayerChar = Character;
    ItemInfoManager = GetGameInstance()->GetSubsystem<UManager_ItemInfo>();

    ItemContainer_Inventory->InitItemContainer();
    ItemContainer_Inventory->SetIconSize(150);
    ItemContainer_Inventory->SetColumnsNum(3);
    ItemContainer_Inventory->SetItemButtenActionType(EItemButtonActionType::EBAT_EquipmentSlot);

    ItemContainer_Storage->InitItemContainer();
    ItemContainer_Storage->SetIconSize(150);
    ItemContainer_Storage->SetColumnsNum(3);
    ItemContainer_Storage->SetItemButtenActionType(EItemButtonActionType::EBAT_EquipmentSlot);

    All->OnClicked.AddDynamic(this, &UUW_Storage::FilterAll);
    Equipment->OnClicked.AddDynamic(this, &UUW_Storage::FilterEquipment);
    Consumable->OnClicked.AddDynamic(this, &UUW_Storage::FilterConsumable);


    ItemContainer_Inventory->ClickedFunc.BindUObject(this, &UUW_Storage::ButtonOnClicked_Inventory);
    ItemContainer_Inventory->HoverdFunc.BindUObject(this, &UUW_Storage::ButtonOnHoverd_Inventory);
    ItemContainer_Inventory->UnHoverdFunc.BindUObject(this, &UUW_Storage::ButtonOnUnHoverd_Inventory);

    ItemContainer_Storage->ClickedFunc.BindUObject(this, &UUW_Storage::ButtonOnClicked_Storage);
    ItemContainer_Storage->HoverdFunc.BindUObject(this, &UUW_Storage::ButtonOnHoverd_Storage);
    ItemContainer_Storage->UnHoverdFunc.BindUObject(this, &UUW_Storage::ButtonOnUnHoverd_Storage);

}

void UUW_Storage::InitUI()
{
    ItemContainer_Inventory->SetFilter(EItemFilter::EIF_ALL);
    ItemContainer_Inventory->ResetGrid();
    ItemContainer_Inventory->SetGrid(InventoryComp->Inventory);

    ItemContainer_Storage->SetFilter(EItemFilter::EIF_ALL);
    ItemContainer_Storage->ResetGrid();
    ItemContainer_Storage->SetGrid(InventoryComp->Storage);
}

void UUW_Storage::FilterAll()
{
    ItemContainer_Inventory->SetFilter(EItemFilter::EIF_ALL);
    ItemContainer_Inventory->ResetGrid();
    ItemContainer_Inventory->SetGrid(InventoryComp->Inventory);

    ItemContainer_Storage->SetFilter(EItemFilter::EIF_ALL);
    ItemContainer_Storage->ResetGrid();
    ItemContainer_Storage->SetGrid(InventoryComp->Storage);
}
void UUW_Storage::FilterEquipment()
{
    ItemContainer_Inventory->SetFilter(EItemFilter::EIF_Equipment);
    ItemContainer_Inventory->ResetGrid();
    ItemContainer_Inventory->SetGrid(InventoryComp->Inventory);

    ItemContainer_Storage->SetFilter(EItemFilter::EIF_Equipment);
    ItemContainer_Storage->ResetGrid();
    ItemContainer_Storage->SetGrid(InventoryComp->Storage);
}

void UUW_Storage::FilterConsumable()
{
    ItemContainer_Inventory->SetFilter(EItemFilter::EIF_Consumable);
    ItemContainer_Inventory->ResetGrid();
    ItemContainer_Inventory->SetGrid(InventoryComp->Inventory);

    ItemContainer_Storage->SetFilter(EItemFilter::EIF_Consumable);
    ItemContainer_Storage->ResetGrid();
    ItemContainer_Storage->SetGrid(InventoryComp->Storage);
}



void UUW_Storage::ButtonOnClicked_Storage(int32 Index, EItemButtonActionType ButtenActionType, FItemData ItemData)
{
    InventoryComp->MoveStorageToInventory(InventoryComp->Storage[Index]);
    ItemContainer_Inventory->ResetGrid();
    ItemContainer_Inventory->SetGrid(InventoryComp->Inventory);
    ItemContainer_Storage->ResetGrid();
    ItemContainer_Storage->SetGrid(InventoryComp->Storage);
}

void UUW_Storage::ButtonOnHoverd_Storage(int32 Index, EItemButtonActionType ButtenActionType, FItemData ItemData)
{
    ItemNameText_Storage->SetText(FText::FromString(ItemInfoManager->GetItemNameString(ItemData)));
    ItemContainer_Inventory->ResetGrid();
    ItemContainer_Inventory->SetGrid(InventoryComp->Inventory);
    ItemContainer_Storage->ResetGrid();
    ItemContainer_Storage->SetGrid(InventoryComp->Storage);
}

void UUW_Storage::ButtonOnUnHoverd_Storage(int32 Index, EItemButtonActionType ButtenActionType, FItemData ItemData)
{
    ItemNameText_Storage->SetText(FText::FromString(""));
}

void UUW_Storage::ButtonOnClicked_Inventory(int32 Index, EItemButtonActionType ButtenActionType, FItemData ItemData)
{
    InventoryComp->MoveInventoryToStorage(InventoryComp->Inventory[Index]);
    ItemContainer_Inventory->ResetGrid();
    ItemContainer_Inventory->SetGrid(InventoryComp->Inventory);
    ItemContainer_Storage->ResetGrid();
    ItemContainer_Storage->SetGrid(InventoryComp->Storage);
}

void UUW_Storage::ButtonOnHoverd_Inventory(int32 Index, EItemButtonActionType ButtenActionType, FItemData ItemData)
{
    ItemNameText_Inventory->SetText(FText::FromString(ItemInfoManager->GetItemNameString(ItemData)));
}

void UUW_Storage::ButtonOnUnHoverd_Inventory(int32 Index, EItemButtonActionType ButtenActionType, FItemData ItemData)
{
    ItemNameText_Inventory->SetText(FText::FromString(""));
}

