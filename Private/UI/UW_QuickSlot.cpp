// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_QuickSlot.h"
#include "UW_ItemObject.h"
#include "PDA_ItemDetailInfo.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"


void UUW_QuickSlot::InitQuickslot()
{
	GettingItemIcon->InitItemObject();
}

void UUW_QuickSlot::SetWeaponSlot(FItemData& Weaponslot)
{
	WeaponSlot->SetItemInfo(Weaponslot);
	GetWorld()->GetTimerManager().SetTimer(WeaponHandle, this, &UUW_QuickSlot::InvisibleItemGetText, 2, false);
}
void UUW_QuickSlot::SetQuickSlot(FItemData& Weaponslot)
{
	QuickSlot->SetItemInfo(Weaponslot);
	GetWorld()->GetTimerManager().SetTimer(QuickSlotHandle, this, &UUW_QuickSlot::InvisibleItemGetText, 2, false);
}

void UUW_QuickSlot::SetSoul(int32 Soul)
{
	FString temp = FString::FromInt(Soul);
	SoulText->SetText(FText::FromString(temp));
}

void UUW_QuickSlot::VisibleItemGetText(FItemData& curItem, FText Text)
{
	GettingItemIcon->SetVisibility(ESlateVisibility::Visible);
	GettingItemIcon->SetItemInfo(curItem);
	GetItemInfo->SetVisibility(ESlateVisibility::Visible);
	GetItemInfoText->SetText(Text);
	GetItemInfoText->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(MassageHandle,this , &UUW_QuickSlot::InvisibleItemGetText, 2 , false);
}
void UUW_QuickSlot::InvisibleItemGetText()
{
	GettingItemIcon->SetVisibility(ESlateVisibility::Collapsed);
	GetItemInfo->SetVisibility(ESlateVisibility::Collapsed);
	GetItemInfoText->SetText(FText::FromString(""));
	GetItemInfoText->SetVisibility(ESlateVisibility::Collapsed);
}
void UUW_QuickSlot::InvisibleWeaponSlot()
{
	WeaponSlot->SetVisibility(ESlateVisibility::Collapsed);
}
void UUW_QuickSlot::InvisibleQuickSlot()
{
	QuickSlot->SetVisibility(ESlateVisibility::Collapsed);
}