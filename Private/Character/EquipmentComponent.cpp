#include "EquipmentComponent.h"
#include "CharacterPlayableBase.h"
#include "WeaponDefaultBase.h"
#include "InventoryComponent.h"
#include "AnimInstanceDefaultBase.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipmentComponent::Initialize(ACharacterPlayableBase* InOwner)
{
	OwnerChar = InOwner;
}

void UEquipmentComponent::UpdateCurrentWeaponSlotUI()
{
	if (!OwnerChar || !OwnerChar->InventoryComp)
	{
		return;
	}

	FItemData Temp = OwnerChar->InventoryComp->GetWeaponSlotWeapon(OwnerChar->CurrentWeaponSlotNum);
	OnWeaponSlotChanged.Broadcast(Temp);
}

void UEquipmentComponent::UpdateCurrentQuickSlotUI()
{
	if (!OwnerChar || !OwnerChar->InventoryComp)
	{
		return;
	}

	FItemData Temp = OwnerChar->InventoryComp->GetQuickSlotItem(OwnerChar->CurrentQuickSlotNum);
	OnQuickSlotChanged.Broadcast(Temp);
}

void UEquipmentComponent::WeaponChange()
{
	if (!OwnerChar || !OwnerChar->GetIsMoveable())
	{
		return;
	}

	EquipMainSlotWeapon((OwnerChar->CurrentWeaponSlotNum + 1) % 3);
}

void UEquipmentComponent::QuickSlotChange()
{
	if (!OwnerChar)
	{
		return;
	}

	EquipQuickSlotItem((OwnerChar->CurrentQuickSlotNum + 1) % 3);
}

void UEquipmentComponent::AddWeaponToMainSlot(TSoftClassPtr<AWeaponDefaultBase> WeaponClass, int32 SlotNum)
{
	if (!OwnerChar || !OwnerChar->GetWorld() || !OwnerChar->InventoryComp)
	{
		return;
	}

	TSubclassOf<AWeaponDefaultBase> SpawnBp = WeaponClass.Get();
	if (!SpawnBp)
	{
		SpawnBp = WeaponClass.LoadSynchronous();
	}
	if (!SpawnBp)
	{
		return;
	}

	if (SlotNum > BaseWeaponSlot)
	{
		return;
	}

	TObjectPtr<AWeaponDefaultBase> TempWeapon = OwnerChar->GetWorld()->SpawnActor<AWeaponDefaultBase>(SpawnBp, FTransform());
	if (!TempWeapon)
	{
		return;
	}

	if (OwnerChar->MainWeaponSlot[SlotNum])
	{
		OwnerChar->MainWeaponSlot[SlotNum]->Destroy();
		OwnerChar->MainWeaponSlot[SlotNum] = nullptr;
	}

	OwnerChar->MainWeaponSlot[SlotNum] = TempWeapon.Get();

	if (FItemData* WeaponData = OwnerChar->InventoryComp->GetWeaponSlotWeaponInfo(SlotNum))
	{
		OwnerChar->MainWeaponSlot[SlotNum]->SetItemData(WeaponData);
	}

	OwnerChar->MainWeaponSlot[SlotNum]->SettingWeaponData(OwnerChar);
	if (SlotNum != OwnerChar->CurrentWeaponSlotNum)
	{
		OwnerChar->MainWeaponSlot[SlotNum]->SetActiveWeapon(false);
	}
	else
	{
		EquipMainSlotWeapon(SlotNum);
	}

	FItemData Temp = OwnerChar->InventoryComp->GetWeaponSlotWeapon(SlotNum);
	OnWeaponSlotChanged.Broadcast(Temp);
}

void UEquipmentComponent::ResetWeaponSlotInfo(int32 SlotNum)
{
	if (!OwnerChar)
	{
		return;
	}

	if (SlotNum > BaseWeaponSlot - 1)
	{
		return;
	}

	if (OwnerChar->MainWeaponSlot[SlotNum])
	{
		OwnerChar->MainWeaponSlot[SlotNum]->Destroy();
		OwnerChar->MainWeaponSlot[SlotNum] = nullptr;
		if (SlotNum == OwnerChar->CurrentWeaponSlotNum)
		{
			EquipMainSlotWeapon(SlotNum);
		}
	}
}

void UEquipmentComponent::EquipQuickSlotItem(int32 SlotNum)
{
	if (!OwnerChar)
	{
		return;
	}

	OwnerChar->CurrentQuickSlotNum = SlotNum;
	UpdateCurrentQuickSlotUI();
}

void UEquipmentComponent::EquipMainSlotWeapon(int32 SlotNum)
{
	if (!OwnerChar || !OwnerChar->InventoryComp)
	{
		return;
	}

	if (SlotNum > BaseWeaponSlot - 1)
	{
		return;
	}

	OwnerChar->CurrentWeaponSlotNum = SlotNum;

	AWeaponDefaultBase* CurrentWeapon = OwnerChar->GetCurrentWeapon();
	if (CurrentWeapon)
	{
		CurrentWeapon->SetActiveWeapon(false);
	}

	if (IsValid(OwnerChar->MainWeaponSlot[OwnerChar->CurrentWeaponSlotNum]))
	{
		OwnerChar->MainWeaponSlot[OwnerChar->CurrentWeaponSlotNum]->SetActiveWeapon(true);
		OwnerChar->SetCurrentWeaponInternal(OwnerChar->MainWeaponSlot[OwnerChar->CurrentWeaponSlotNum]);
	}
	else
	{
		OwnerChar->SetCurrentWeaponInternal(OwnerChar->MainWeaponSlot[3]);
		if (AWeaponDefaultBase* NewWeapon = OwnerChar->GetCurrentWeapon())
		{
			NewWeapon->SetActiveWeapon(true);
		}
	}

	UAnimInstanceDefaultBase* AnimInstance = OwnerChar->GetAnimInstance();
	AWeaponDefaultBase* NewWeapon = OwnerChar->GetCurrentWeapon();
	if (AnimInstance && NewWeapon && !NewWeapon->bIsTwoHanded)
	{
		AnimInstance->SetIsTwoHandedWeapon(false);
	}

	if (AnimInstance && NewWeapon)
	{
		if (NewWeapon->IdleMotion) AnimInstance->WeaponIdleMotion = NewWeapon->IdleMotion;
		if (NewWeapon->WalkMotion) AnimInstance->WeaponWalkMotion = NewWeapon->WalkMotion;
		if (NewWeapon->RunMotion)  AnimInstance->WeaponRunMotion  = NewWeapon->RunMotion;
	}

	FItemData Temp = OwnerChar->InventoryComp->GetWeaponSlotWeapon(OwnerChar->CurrentWeaponSlotNum);
	OnWeaponSlotChanged.Broadcast(Temp);
}

