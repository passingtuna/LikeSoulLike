// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_BoneFireMenu.h"
#include "Manager_UI.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Manager_Bonefire.h"



void UUW_BoneFireMenu::InitBonefireMenu()
{
	UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UManager_UI>();
	BonefireManager = GetWorld()->GetGameInstance()->GetSubsystem<UManager_Bonefire>();
	Storage->OnClicked.AddDynamic(this, &UUW_BoneFireMenu::ClickedStorageButton);
	LevelUp->OnClicked.AddDynamic(this, &UUW_BoneFireMenu::ClickedLevelUpButton);
	Warp->OnClicked.AddDynamic(this, &UUW_BoneFireMenu::ClickedWarpButton);
}

void UUW_BoneFireMenu::ClickedLevelUpButton()
{
	UIManager->ShowUIByName("Status");
}
void UUW_BoneFireMenu::ClickedWarpButton()
{
	UIManager->ShowUIByName("BonefireWarp");
}
void UUW_BoneFireMenu::ClickedStorageButton()
{
	UIManager->ShowUIByName("Storage");
}

void UUW_BoneFireMenu::SetBonefireName(FName name)
{
	BonefireName = name;
	BonefireNameText->SetText(FText::FromString(BonefireName.ToString()));
}