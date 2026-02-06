// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_StatusWindow.h"
#include "CharacterPlayableBase.h"
#include "WeaponDefaultBase.h"
#include "Manager_Calculate.h"
#include "Manager_UI.h"


void UUW_StatusWindow::NativeConstruct()
{
	Super::NativeConstruct();

	StatusEditBox->OnEditBoxChanged.AddUObject(
		this, &UUW_StatusWindow::OnStatusEdited);
	WeaponEditBox->OnEditBoxChanged.AddUObject(
		this, &UUW_StatusWindow::OnStatusEdited);


	WeaponEditBox->BindDelegates();

}
void UUW_StatusWindow_StatusEditBox::NativeConstruct()
{
	BindDelegates();

}
void UUW_StatusWindow_WeaponEditBar::NativeConstruct()
{
	InfusionInfo->OnSelectionChanged.AddDynamic(this, &UUW_StatusWindow_WeaponEditBar::UpdatedInfusion);
}


void UUW_StatusWindow::InitUI()
{

	CalculatManager = GetWorld()->GetGameInstance()->GetSubsystem<UManager_Calculate>();
	TargetCharacter = GetWorld()->GetGameInstance()->GetSubsystem<UManager_UI>()->GetPlayerCharacter();
	StatusEditBox->SetStatusEditBox(TargetCharacter->GetCurrentStatus());
	StatusEditBox->CumulativeSoulNum = 0;
	StatusEditBox->UpdateCumulativeSoul();
	int32 needSoul = CalculatManager->GetNextRequireSoul(TargetCharacter->GetCurrentStatus().Level);
	StatusEditBox->SetRequireSoul(needSoul);
	if (TargetCharacter->CurrentStatus.Soul < needSoul)
	{
		StatusEditBox->DeativateButton = true;
	}
	else
	{
		StatusEditBox->DeativateButton = false;
	}

	WeaponSlot1 = (TargetCharacter->GetMainWeaponSlot(0));
	if(WeaponSlot1)
	{
		WeaponEditBox->WeaponEditSlot1->SetVisibility(ESlateVisibility::Visible);
		int32 Upgrade = 0;
		EWeaponInfusionType Infusion = EWeaponInfusionType::EWIT_None;
		FItemData* WeaponData = WeaponSlot1->GetItemData();
		if (WeaponData)
		{
			Upgrade = WeaponData->Upgrade;
			Infusion = WeaponData->Infusion;
		}
		WeaponEditBox->SetStatusBox(1, Upgrade, Infusion);
		WeaponSlotBox1->SetSlotNum(1);
		WeaponSlotBox1->SetWeaponInfoBox(CalculatManager->GetWeaponBaseDamage(WeaponSlot1, Upgrade, Infusion, TargetCharacter->GetCurrentStatus()));
	}
	else
	{
		FDamageData WeaponDamage;
		WeaponSlotBox1->SetWeaponInfoBox(WeaponDamage);
		WeaponEditBox->WeaponEditSlot1->SetVisibility(ESlateVisibility::Collapsed);
	}


	WeaponSlot2 = (TargetCharacter->GetMainWeaponSlot(1));
	if (WeaponSlot2)
	{
		WeaponEditBox->WeaponEditSlot2->SetVisibility(ESlateVisibility::Visible);
		int32 Upgrade = 0;
		EWeaponInfusionType Infusion = EWeaponInfusionType::EWIT_None;
		FItemData* WeaponData = WeaponSlot2->GetItemData();
		if (WeaponData)
		{
			Upgrade = WeaponData->Upgrade;
			Infusion = WeaponData->Infusion;
		}
		WeaponEditBox->SetStatusBox(2, Upgrade, Infusion);
		WeaponSlotBox2->SetSlotNum(2);
		WeaponSlotBox2->SetWeaponInfoBox(CalculatManager->GetWeaponBaseDamage(WeaponSlot2, Upgrade, Infusion, TargetCharacter->GetCurrentStatus()));
	}
	else
	{
		FDamageData WeaponDamage;
		WeaponSlotBox2->SetWeaponInfoBox(WeaponDamage);
		WeaponEditBox->WeaponEditSlot2->SetVisibility(ESlateVisibility::Collapsed);
	}


	WeaponSlot3 = (TargetCharacter->GetMainWeaponSlot(2));
	if (WeaponSlot3)
	{
		WeaponEditBox->WeaponEditSlot3->SetVisibility(ESlateVisibility::Visible);
		int32 Upgrade = 0;
		EWeaponInfusionType Infusion = EWeaponInfusionType::EWIT_None;
		FItemData* WeaponData = WeaponSlot3->GetItemData();
		if (WeaponData)
		{
			Upgrade = WeaponData->Upgrade;
			Infusion = WeaponData->Infusion;
		}

		WeaponEditBox->SetStatusBox(3, Upgrade, Infusion);
		WeaponSlotBox3->SetSlotNum(3);
		WeaponSlotBox3->SetWeaponInfoBox(CalculatManager->GetWeaponBaseDamage(WeaponSlot3, Upgrade, Infusion, TargetCharacter->GetCurrentStatus()));
	}
	else
	{
		FDamageData WeaponDamage;
		WeaponSlotBox3->SetWeaponInfoBox(WeaponDamage);
		WeaponEditBox->WeaponEditSlot3->SetVisibility(ESlateVisibility::Collapsed);
	}


	StateBox->SetStateInfoBox(TargetCharacter->GetCurrentState());
	ResistBox->SetResistInfoBox(TargetCharacter->GetCurrentResist());
}

void UUW_StatusWindow::ClickedConfirmButton()
{
	if (StatusEditBox->IsEdited())
	{
		FPlayerStatus tempStatus = StatusEditBox->GetEditedStatus();
		tempStatus.Soul = TargetCharacter->CurrentStatus.Soul;
		TargetCharacter->CurrentStatus = tempStatus;
		TargetCharacter->ModifyCurrentSoul(StatusEditBox->CumulativeSoulNum * -1);
	}
	if (WeaponEditBox->WeaponEditSlot1->IsEdited())
	{
		if (WeaponSlot1)
		{
			WeaponSlot1->SetUpgrade(WeaponEditBox->WeaponEditSlot1->GetUpgrade());
			WeaponSlot1->SetInfusion((EWeaponInfusionType)WeaponEditBox->WeaponEditSlot1->GetInfusion());
		}
	}

	if (WeaponEditBox->WeaponEditSlot2->IsEdited())
	{
		if (WeaponSlot2)
		{
			WeaponSlot2->SetUpgrade(WeaponEditBox->WeaponEditSlot2->GetUpgrade());
			WeaponSlot2->SetInfusion((EWeaponInfusionType)WeaponEditBox->WeaponEditSlot2->GetInfusion());
		}
	}
	if (WeaponEditBox->WeaponEditSlot3->IsEdited())
	{
		if (WeaponSlot3)
		{
			WeaponSlot3->SetUpgrade(WeaponEditBox->WeaponEditSlot3->GetUpgrade());
			WeaponSlot3->SetInfusion((EWeaponInfusionType)WeaponEditBox->WeaponEditSlot3->GetInfusion());
		}
	}
	TargetCharacter->UpdateStatus();
	GetWorld()->GetGameInstance()->GetSubsystem<UManager_UI>()->PopUI();
}

void UUW_StatusWindow::ClickedCancleButton()
{
	GetWorld()->GetGameInstance()->GetSubsystem<UManager_UI>()->PopUI();
}

void UUW_StatusWindow::OnStatusEdited()
{
	FPlayerStatus tempStatus = StatusEditBox->GetEditedStatus();

	int32 needSoul = CalculatManager->GetNextRequireSoul(tempStatus.Level);
	StatusEditBox->SetRequireSoul(needSoul);

	StateBox->SetStateInfoBox(CalculatManager->CalculateMaxState(tempStatus));
	ResistBox->SetResistInfoBox(CalculatManager->CalculateMaxResist(tempStatus));

	int32 weaponUp = WeaponEditBox->GetEditedUpgrade(1);
	EWeaponInfusionType weaponInfusion = (EWeaponInfusionType)WeaponEditBox->GetEditedInfusion(1);
	WeaponSlotBox1->SetWeaponInfoBox(CalculatManager->GetWeaponBaseDamage(WeaponSlot1, weaponUp, weaponInfusion,tempStatus));

	weaponUp = WeaponEditBox->GetEditedUpgrade(2);
	weaponInfusion = (EWeaponInfusionType)WeaponEditBox->GetEditedInfusion(2);
	WeaponSlotBox2->SetWeaponInfoBox(CalculatManager->GetWeaponBaseDamage(WeaponSlot2, weaponUp, weaponInfusion,tempStatus));

	weaponUp = WeaponEditBox->GetEditedUpgrade(3);
	weaponInfusion = (EWeaponInfusionType)WeaponEditBox->GetEditedInfusion(3);
	WeaponSlotBox3->SetWeaponInfoBox(CalculatManager->GetWeaponBaseDamage(WeaponSlot3, weaponUp, weaponInfusion,tempStatus));

	StatusEditBox->CumulativeSoulNum = CalculateCumulativeSoul(TargetCharacter->GetCurrentStatus().Level, tempStatus.Level);
	StatusEditBox->UpdateCumulativeSoul();

	if (TargetCharacter->CurrentStatus.Soul < (StatusEditBox->CumulativeSoulNum + needSoul))
	{
		StatusEditBox->DeativateButton = true;
	}
	else
	{
		StatusEditBox->DeativateButton = false;
	}
}

int32 UUW_StatusWindow::CalculateCumulativeSoul(int32 FromLevel, int32 ToLevel)
{
	int32 Total = 0;
	for (int32 i = FromLevel; i < ToLevel; ++i)
	{
		Total += CalculatManager->GetNextRequireSoul(i);
	}
	return Total;
}


void UUW_StatusWindow_StatusEditBar::ClickedAddButton()
{
	if (StatusNum >= 99) return;
	if (EditBox->DeativateButton) return;
	StatusNum++;
	EditBox->ModifyLevel(+1);
	SetStatusNum(StatusNum);
	OnEditBarChanged.Broadcast();
};
void UUW_StatusWindow_StatusEditBar::ClickedMinusButton()
{
	if (StatusNum <= InitStateNum) return;
	StatusNum--;
	EditBox->ModifyLevel(-1);
	SetStatusNum(StatusNum);
	OnEditBarChanged.Broadcast();
};