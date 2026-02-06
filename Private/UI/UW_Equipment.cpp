// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_Equipment.h"
#include "InventoryComponent.h"
#include "Manager_ItemInfo.h"
#include "UW_ItemObject.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "CharacterPlayableBase.h"
#include "UW_ItemContainer.h"

void UUW_Equipment::InitUI()
{
    QuickSlot1->SetItemInfo(InventoryComp->GetQuickSlotItem(0));
    QuickSlot2->SetItemInfo(InventoryComp->GetQuickSlotItem(1));
    QuickSlot3->SetItemInfo(InventoryComp->GetQuickSlotItem(2));

    WeaponSlot1->SetItemInfo(InventoryComp->GetWeaponSlotWeapon(0));
    WeaponSlot2->SetItemInfo(InventoryComp->GetWeaponSlotWeapon(1));
    WeaponSlot3->SetItemInfo(InventoryComp->GetWeaponSlotWeapon(2));
    ItemContainer->SetFilter(EItemFilter::EIF_None);
    ItemContainer->IsEmptyObject = true;
}

void UUW_Equipment::InitEquipment(ACharacterPlayableBase* Character)
{
	InventoryComp = Character->GetInventoryComponent();
    PlayerChar = Character;
	ItemInfoManager = GetGameInstance()->GetSubsystem<UManager_ItemInfo>();

    ItemContainer->InitItemContainer();
    ItemContainer->SetIconSize(150);
    ItemContainer->SetColumnsNum(4);
    ItemContainer->SetItemButtenActionType(EItemButtonActionType::EBAT_EquipmentSlot);

    ItemContainer->ClickedFunc.BindUObject(this, &UUW_Equipment::ButtonOnClicked);
    ItemContainer->HoverdFunc.BindUObject(this, &UUW_Equipment::ButtonOnHoverd);
    ItemContainer->UnHoverdFunc.BindUObject(this, &UUW_Equipment::ButtonOnUnHoverd);


    QuickSlot1->InitItemObject();
    QuickSlot1->OnItemClicked.BindUObject(this, &UUW_Equipment::ButtonOnClicked);
    QuickSlot1->SetButtenIndexType(1, EItemButtonActionType::EBAT_QucikSlot);

    QuickSlot2->InitItemObject();
    QuickSlot2->OnItemClicked.BindUObject(this, &UUW_Equipment::ButtonOnClicked);
    QuickSlot2->SetButtenIndexType(2, EItemButtonActionType::EBAT_QucikSlot);

    QuickSlot3->InitItemObject();
    QuickSlot3->OnItemClicked.BindUObject(this, &UUW_Equipment::ButtonOnClicked);
    QuickSlot3->SetButtenIndexType(3, EItemButtonActionType::EBAT_QucikSlot);

    WeaponSlot1->InitItemObject();
    WeaponSlot1->OnItemClicked.BindUObject(this,&UUW_Equipment::ButtonOnClicked);
    WeaponSlot1->SetButtenIndexType(1, EItemButtonActionType::EBAT_WeaponSlot);

    WeaponSlot2->InitItemObject();
    WeaponSlot2->OnItemClicked.BindUObject(this, &UUW_Equipment::ButtonOnClicked);
    WeaponSlot2->SetButtenIndexType(2, EItemButtonActionType::EBAT_WeaponSlot);

    WeaponSlot3->InitItemObject();
    WeaponSlot3->OnItemClicked.BindUObject(this, &UUW_Equipment::ButtonOnClicked);
    WeaponSlot3->SetButtenIndexType(3, EItemButtonActionType::EBAT_WeaponSlot);

}


void UUW_Equipment::ButtonOnClicked(int32 Index, EItemButtonActionType ButtenActionType, FItemData ItemData)
{
    switch (ButtenActionType)
    {
        case EItemButtonActionType::EBAT_QucikSlot:
            SelectedIndex = Index;
            SelectedSlot = ButtenActionType;
            ItemContainer->SetFilter(EItemFilter::EIF_Consumable);
            //FilterType = EItemFilter::EIF_Consumable;
            ItemContainer->SetGrid(InventoryComp->Inventory);
            break;
        case EItemButtonActionType::EBAT_WeaponSlot:
            SelectedIndex = Index;
            SelectedSlot = ButtenActionType;
            ItemContainer->SetFilter(EItemFilter::EIF_Equipment);
            //FilterType = EItemFilter::EIF_Equipment;
            ItemContainer->SetGrid(InventoryComp->Inventory);
            break;
        case EItemButtonActionType::EBAT_EquipmentSlot:
            EquipItem(Index , ItemData);
            break;
    }
}
void UUW_Equipment::EquipItem(int32 Index, FItemData ItemData)
{
    ItemInfoManager->LoadNoneSyncItemInfo(ItemData.ID);
    switch (SelectedSlot)
    {
        case EItemButtonActionType::EBAT_QucikSlot:
        {
            switch (SelectedIndex)
            {
            case 1:
                InventoryComp->EquipQuickSlot(0, Index);
                break;
            case 2:
                InventoryComp->EquipQuickSlot(1, Index);
                break;
            case 3:
                InventoryComp->EquipQuickSlot(2, Index);
                break;
            default:
                break;
            }
            PlayerChar->UpdateCurrentQuickSlotUI();
        }
        break;
        case EItemButtonActionType::EBAT_WeaponSlot:
        {
            switch (SelectedIndex)
            {
            case 1:
                InventoryComp->EquipWeapon(0, Index);
                break;
            case 2:
                InventoryComp->EquipWeapon(1, Index);
                break;
            case 3:
                InventoryComp->EquipWeapon(2, Index);
                break;
            default:
                return;
                break;
            }
            PlayerChar->UpdateCurrentWeaponSlotUI();
        }
        break;
        default:break;
    }
    InitUI();
    ItemContainer->ResetGrid();
}

void UUW_Equipment::ButtonOnHoverd(int32 Index, EItemButtonActionType ButtenActionType, FItemData ItemData)
{
    ItemNameText->SetText(FText::FromString(ItemInfoManager->GetItemNameString(ItemData)));
}

void UUW_Equipment::ButtonOnUnHoverd(int32 Index, EItemButtonActionType ButtenActionType, FItemData ItemData)
{
    ItemNameText->SetText(FText::FromString(""));
}

