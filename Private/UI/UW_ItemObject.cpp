// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_ItemObject.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Manager_ItemInfo.h"
#include "PDA_ItemDetailInfo.h"


void UUW_ItemObject::NativeConstruct()
{
}
void UUW_ItemObject::InitItemObject()
{
    CountBox->SetVisibility(ESlateVisibility::Collapsed);
    Count->SetVisibility(ESlateVisibility::Collapsed);

    EquipBox->SetVisibility(ESlateVisibility::Collapsed);
    Equip->SetVisibility(ESlateVisibility::Collapsed);

    Button->OnClicked.AddDynamic(this, &UUW_ItemObject::OnClickedItem);
    Button->OnHovered.AddDynamic(this, &UUW_ItemObject::OnHoveredItem);
    Button->OnUnhovered.AddDynamic(this, &UUW_ItemObject::OnUnhoveredItem);

}

void UUW_ItemObject::SetItemInfo(FItemData itemData)
{
    UManager_ItemInfo* ItemInfoManager = GetGameInstance()->GetSubsystem<UManager_ItemInfo>();
    if (!ItemImage) return;

    if (itemData.ID == NAME_None)
    {
        CountBox->SetVisibility(ESlateVisibility::Collapsed);
        Count->SetVisibility(ESlateVisibility::Collapsed);
        EquipBox->SetVisibility(ESlateVisibility::Collapsed);
        Equip->SetVisibility(ESlateVisibility::Collapsed);

        FIconData tempIconData = ItemInfoManager->GetIconByID(itemData.ID);
        ItemImage->SetBrushFromTexture(tempIconData.Atlas);
        FSlateBrush Brush = ItemImage->GetBrush();
        Brush.SetUVRegion(FBox2D(tempIconData.Uv, tempIconData.Uv + tempIconData.IconSizeNormalized));
        ItemImage->SetBrush(Brush);
       return;
    }
    ItemData = itemData;
    FIconData tempIconData = ItemInfoManager->GetIconByID(ItemData.ID);
    if (tempIconData.Atlas == nullptr) return;
    ItemImage->SetBrushFromTexture(tempIconData.Atlas);
    FSlateBrush Brush = ItemImage->GetBrush();
    Brush.SetUVRegion(FBox2D(tempIconData.Uv, tempIconData.Uv + tempIconData.IconSizeNormalized));
    ItemImage->SetBrush(Brush);

    FItemBaseData ItemBaseData = ItemInfoManager->GetItemBaseData(ItemData.ID);
    if (ItemBaseData.MaxStackCount > 1)
    {
        CountBox->SetVisibility(ESlateVisibility::Visible); 
        Count->SetVisibility(ESlateVisibility::Visible);
        FString CountStr = FString::FromInt(ItemData.Count);
        Count->SetText(FText::FromString(CountStr));
    }
    else
    {
        CountBox->SetVisibility(ESlateVisibility::Collapsed);
        Count->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (ItemData.IsEquiped)
    {
        EquipBox->SetVisibility(ESlateVisibility::Visible);
        Equip->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        EquipBox->SetVisibility(ESlateVisibility::Collapsed);
        Equip->SetVisibility(ESlateVisibility::Collapsed);
    }
}
void UUW_ItemObject::OnClickedItem()
{
    if (OnItemClicked.IsBound())
    {
        OnItemClicked.Execute(Index, ButtenActionType, ItemData);
    }
}

void UUW_ItemObject::OnHoveredItem()
{
    if (OnItemHovered.IsBound())
    {
        OnItemHovered.Execute(Index, ButtenActionType, ItemData);
    }
}

void UUW_ItemObject::OnUnhoveredItem()
{
    if (OnItemUnHovered.IsBound())
    {
        OnItemUnHovered.Execute(Index, ButtenActionType, ItemData);
    }
}