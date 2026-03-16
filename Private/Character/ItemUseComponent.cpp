#include "ItemUseComponent.h"
#include "CharacterPlayableBase.h"
#include "InventoryComponent.h"
#include "Manager_ItemInfo.h"
#include "AnimInstanceDefaultBase.h"

UItemUseComponent::UItemUseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UItemUseComponent::Initialize(ACharacterPlayableBase* InOwner)
{
	OwnerChar = InOwner;
}

void UItemUseComponent::HandleUseItem()
{
	if (!OwnerChar || !OwnerChar->GetIsMoveable())
	{
		return;
	}

	OwnerChar->SetCrouchState(false);
	if (!OwnerChar->GetIsMoveable())
	{
		return;
	}

	UInventoryComponent* InventoryComp = OwnerChar->GetInventoryComponent();
	UManager_ItemInfo* ItemInfoManager = OwnerChar->GetItemInfoManager();
	if (!InventoryComp || !ItemInfoManager)
	{
		return;
	}

	const int32 QuickSlotIndex = OwnerChar->GetCurrentQuickSlotIndex();
	FItemData* ItemInfo = InventoryComp->GetQuickSlotItemInfo(QuickSlotIndex);
	if (!ItemInfo)
	{
		return;
	}

	const FItemBaseData BaseData = ItemInfoManager->GetItemBaseData(ItemInfo->ID);
	if (ItemInfo->Count < 1)
	{
		return;
	}

	if (BaseData.ItemType != EItemType::IT_Refillable &&
		BaseData.ItemType != EItemType::IT_Consumable)
	{
		return;
	}

	UAnimInstanceDefaultBase* AnimInstance = OwnerChar->GetAnimInstance();
	if (AnimInstance)
	{
		if (UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage())
		{
			if (CurrentMontage->GetName() == "UsePotion_Montage" || CurrentMontage->GetName() == "UsePaper")
			{
				return;
			}
		}
	}

	FItemAffectData AffectData = BaseData.ItemAffectData;
	if (!OwnerChar->ExcuteItemAffect(AffectData))
	{
		return;
	}

	OwnerChar->ReadyToAction();
	InventoryComp->ConsumeItem(ItemInfo, 1);
	OwnerChar->UpdateCurrentQuickSlotUI();
}

