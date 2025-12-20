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
	StatusEditBox->SetRequireSoul(CalculatManager->GetNextRequireSoul(TargetCharacter->GetCurrentStatus().Level));

	WeaponSlot1 = (TargetCharacter->GetMainWeaponSlot(0));
	if(WeaponSlot1)
	{
		UE_LOG(LogTemp, Warning, TEXT("업데이트 웨펀 1"));
		WeaponEditBox->SetStatusBox(1, WeaponSlot1->GetUpgrade(), (EWeaponInfusionType)WeaponSlot1->GetInfusionType());
		WeaponSlotBox1->SetSlotNum(1);
		WeaponSlotBox1->SetWeaponInfoBox(CalculatManager->GetWeaponBaseDamage(WeaponSlot1, WeaponSlot1->GetUpgrade(), WeaponSlot1->GetInfusionType(), TargetCharacter->GetCurrentStatus()));
	}

	WeaponSlot2 = (TargetCharacter->GetMainWeaponSlot(1));
	if (WeaponSlot2)
	{

		UE_LOG(LogTemp, Warning, TEXT("업데이트 웨펀 2"));
		WeaponEditBox->SetStatusBox(2, WeaponSlot2->GetUpgrade(), (EWeaponInfusionType)WeaponSlot2->GetInfusionType());
		WeaponSlotBox2->SetSlotNum(2);
		WeaponSlotBox2->SetWeaponInfoBox(CalculatManager->GetWeaponBaseDamage(WeaponSlot2, WeaponSlot2->GetUpgrade(), WeaponSlot2->GetInfusionType(), TargetCharacter->GetCurrentStatus()));
	}

	WeaponSlot3 = (TargetCharacter->GetMainWeaponSlot(2));
	if (WeaponSlot3)
	{
		UE_LOG(LogTemp, Warning, TEXT("업데이트 웨펀 3"));
		WeaponEditBox->SetStatusBox(3, WeaponSlot3->GetUpgrade(), (EWeaponInfusionType)WeaponSlot3->GetInfusionType());
		WeaponSlotBox3->SetSlotNum(3);
		WeaponSlotBox3->SetWeaponInfoBox(CalculatManager->GetWeaponBaseDamage(WeaponSlot3, WeaponSlot3->GetUpgrade(), WeaponSlot3->GetInfusionType(), TargetCharacter->GetCurrentStatus()));
	}

	StateBox->SetStateInfoBox(TargetCharacter->GetCurrentState());
	ResistBox->SetResistInfoBox(TargetCharacter->GetCurrentResist());
}
void UUW_StatusWindow::ClickedConfirmButton()
{
	if (StatusEditBox->IsEdited())
	{
		FPlayerStatus tempStatus = StatusEditBox->GetEditedStatus();
		TargetCharacter->CurrentStatus = tempStatus;
	}
	if (WeaponEditBox->WeaponEditSlot1->IsEdited())
	{
		WeaponSlot1->SetUpgrade(WeaponEditBox->WeaponEditSlot1->GetUpgrade());
		WeaponSlot1->SetInfusion(WeaponEditBox->WeaponEditSlot1->GetInfusion());
	}

	if (WeaponEditBox->WeaponEditSlot2->IsEdited())
	{
		WeaponSlot2->SetUpgrade(WeaponEditBox->WeaponEditSlot1->GetUpgrade());
		WeaponSlot2->SetInfusion(WeaponEditBox->WeaponEditSlot1->GetInfusion());

	}
	if (WeaponEditBox->WeaponEditSlot3->IsEdited())
	{
		WeaponSlot3->SetUpgrade(WeaponEditBox->WeaponEditSlot1->GetUpgrade());
		WeaponSlot3->SetInfusion(WeaponEditBox->WeaponEditSlot1->GetInfusion());
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

	StatusEditBox->SetRequireSoul(CalculatManager->GetNextRequireSoul(tempStatus.Level));

	StateBox->SetStateInfoBox(CalculatManager->CalculateMaxState(tempStatus));
	ResistBox->SetResistInfoBox(CalculatManager->CalculateMaxResist(tempStatus));

	int weaponUp = WeaponEditBox->GetEditedUpgrade(1);
	EWeaponInfusionType weaponInfusion = (EWeaponInfusionType)WeaponEditBox->GetEditedInfusion(1);
	WeaponSlotBox1->SetWeaponInfoBox(CalculatManager->GetWeaponBaseDamage(WeaponSlot1, weaponUp, weaponInfusion,tempStatus));

	weaponUp = WeaponEditBox->GetEditedUpgrade(2);
	weaponInfusion = (EWeaponInfusionType)WeaponEditBox->GetEditedInfusion(2);
	WeaponSlotBox2->SetWeaponInfoBox(CalculatManager->GetWeaponBaseDamage(WeaponSlot2, weaponUp, weaponInfusion,tempStatus));

	weaponUp = WeaponEditBox->GetEditedUpgrade(3);
	weaponInfusion = (EWeaponInfusionType)WeaponEditBox->GetEditedInfusion(3);
	WeaponSlotBox3->SetWeaponInfoBox(CalculatManager->GetWeaponBaseDamage(WeaponSlot3, weaponUp, weaponInfusion,tempStatus));
}



void UUW_StatusWindow_StatusEditBar::ClickedAddButton()
{
	if (StatusNum >= 99) return;
	StatusNum++;
	OnEditBarChanged.Broadcast();
	EditBox->ModifyLevel(+1);
	SetStatusNum(StatusNum);
};
void UUW_StatusWindow_StatusEditBar::ClickedMinusButton()
{
	if (StatusNum <= InitStateNum) return;
	StatusNum--;
	EditBox->ModifyLevel(-1);
	OnEditBarChanged.Broadcast();
	SetStatusNum(StatusNum);
};