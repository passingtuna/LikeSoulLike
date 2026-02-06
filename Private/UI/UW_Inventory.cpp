// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_Inventory.h"	
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "InventoryComponent.h"
#include "Manager_ItemInfo.h"
#include "UW_ItemObject.h"
#include "PDA_ItemDetailInfo.h"
#include "UW_ItemContainer.h"


void UUW_Inventory::InitInventory(UInventoryComponent* Inventory)
{ 
	InventoryComp = Inventory;
	ItemInfoManager = GetGameInstance()->GetSubsystem<UManager_ItemInfo>();
    All->OnClicked.AddDynamic(this, &UUW_Inventory::FilterAll);
    Equipment->OnClicked.AddDynamic(this, &UUW_Inventory::FilterEquipment);
    Consumable->OnClicked.AddDynamic(this, &UUW_Inventory::FilterConsumable);

    ItemContainer->InitItemContainer();
    ItemContainer->SetIconSize(150);
    ItemContainer->SetColumnsNum(7);
    ItemContainer->SetItemButtenActionType(EItemButtonActionType::EBAT_EquipmentSlot);

    ItemContainer->HoverdFunc.BindUObject(this ,&UUW_Inventory::ButtonOnHoverd);
    ItemContainer->UnHoverdFunc.BindUObject(this, &UUW_Inventory::ButtonOnUnHoverd);

}
void UUW_Inventory::InitUI()
{
    ItemContainer->SetFilter(EItemFilter::EIF_ALL);
    ItemContainer->ResetGrid();
    ItemContainer->SetGrid(InventoryComp->Inventory);
}

void UUW_Inventory::FilterAll() 
{
    ItemContainer->SetFilter(EItemFilter::EIF_ALL);
    ItemContainer->ResetGrid();
    ItemContainer->SetGrid(InventoryComp->Inventory);
}
void UUW_Inventory::FilterEquipment()
{
    ItemContainer->SetFilter(EItemFilter::EIF_Equipment);
    ItemContainer->ResetGrid();
    ItemContainer->SetGrid(InventoryComp->Inventory);
}
void UUW_Inventory::FilterConsumable()
{
    ItemContainer->SetFilter(EItemFilter::EIF_Consumable);
    ItemContainer->ResetGrid();
    ItemContainer->SetGrid(InventoryComp->Inventory);
}


void UUW_Inventory::ButtonOnHoverd(int32 Index, EItemButtonActionType ButtenActionType, FItemData ItemData)
{
    ItemNameText->SetText(FText::FromString(ItemInfoManager->GetItemNameString(ItemData)));
}
void UUW_Inventory::ButtonOnUnHoverd(int32 Index, EItemButtonActionType ButtenActionType, FItemData ItemData)
{
    ItemNameText->SetText(FText::FromString(""));
}
