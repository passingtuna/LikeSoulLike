// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_UserMenu.h"
#include "Manager_UI.h"

void UUW_UserMenu::InitUserMenu()
{
	UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UManager_UI>();
}
void UUW_UserMenu::ClickedInventoryButton()
{
	UIManager->ShowUIByName("Inventory");
}
void UUW_UserMenu::ClickedQuickSlotButton()
{
	UIManager->ShowUIByName("Equipment");
}
void UUW_UserMenu::ClickedOptionButton()
{
	UIManager->ShowUIByName("Option");
}