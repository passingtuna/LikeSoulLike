// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_ItemContainer.h"
#include "Manager_ItemInfo.h"
#include "InventoryComponent.h"
#include "UW_ItemObject.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "CharacterPlayableBase.h"

void UUW_ItemContainer::InitItemContainer()
{
    ItemInfoManager = GetGameInstance()->GetSubsystem<UManager_ItemInfo>();
}

void UUW_ItemContainer::SetGrid(TArray<FItemData>& showItems)
{
    int32 numColumns = ColumnsNum;
    int32 itemIndex = 0;
    for (int i = 0; i < showItems.Num(); ++i)
    {
        FItemBaseData temp = ItemInfoManager->GetItemBaseData(showItems[i].ID);
        if (showItems[i].Count <= 0) continue;
        switch (temp.ItemType)
        {
        case EItemType::IT_Weapon:
        case EItemType::IT_Armor:
            if (FilterType != EItemFilter::EIF_ALL && FilterType != EItemFilter::EIF_Equipment) continue;
            break;
        case EItemType::IT_Consumable:
        case EItemType::IT_Refillable:
            if (FilterType != EItemFilter::EIF_ALL && FilterType != EItemFilter::EIF_Consumable) continue;
            break;
        }
        UUW_ItemObject* tempItemObject;
        if (arrItemObjecWidget.Num() <= i)
        {
            tempItemObject = CreateWidget<UUW_ItemObject>(GetWorld(), ItemObject);
            tempItemObject->InitItemObject();
            arrItemObjecWidget.Add(tempItemObject);
        }
        else
        {
            tempItemObject = arrItemObjecWidget[i];
            tempItemObject->SetVisibility(ESlateVisibility::Visible);
        }
        tempItemObject->SetButtenIndexType(i, ItemButtenActionType);
        tempItemObject->SetItemInfo(showItems[i]);
        if (ClickedFunc.IsBound()) tempItemObject->OnItemClicked = ClickedFunc;
        if (HoverdFunc.IsBound()) tempItemObject->OnItemHovered = HoverdFunc;
        if (UnHoverdFunc.IsBound()) tempItemObject->OnItemUnHovered = UnHoverdFunc;
        USizeBox* sizeBox = NewObject<USizeBox>(this);
        sizeBox->SetWidthOverride(Size);
        sizeBox->SetWidthOverride(Size);
        sizeBox->AddChild(tempItemObject);
        UCanvasPanelSlot* slot = CavasPanel->AddChildToCanvas(sizeBox);
        slot->SetSize(FVector2D(Size, Size));
        int32 row = itemIndex / numColumns;
        int32 column = itemIndex % numColumns;
        float spacing = 10.f;
        slot->SetPosition(FVector2D(column * (Size + spacing), row * (Size + spacing)));
        itemIndex++;
    }
}
void UUW_ItemContainer::ResetGrid()
{
    for (auto& Elem : arrItemObjecWidget)
    {
        Elem->SetVisibility(ESlateVisibility::Collapsed);
    }
}
