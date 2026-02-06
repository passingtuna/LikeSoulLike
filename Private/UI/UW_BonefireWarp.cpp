// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_BonefireWarp.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "CharacterPlayableBase.h"
#include "Manager_Bonefire.h"
#include "Manager_UI.h"


void UUW_BonefireWarp::InitBonefireWarpWindow(ACharacterPlayableBase * pc)
{
	PlayerCharacter = pc;
	
}
void UUW_BonefireWarp::InitUI()
{
	Super::InitUI();
	SettingWarpButton();
}

void UUW_BonefireWarp::SettingWarpButton()
{
	WarpButtonList->ClearChildren();

	TSet<FName> TempBonfireList = GetWorld()->GetGameInstance()->GetSubsystem<UManager_Bonefire>()->GetActivateBonfireList();
	int RawNum = 0;
	for (auto & Elem : TempBonfireList)
	{
		UUW_BonefireWarpButton* tempWarpObject;
		if (RawNum <= arrWarpButtonWidget.Num())
		{
			tempWarpObject = CreateWidget<UUW_BonefireWarpButton>(GetWorld(), WarpButtonBP);
			FMargin tmep = (3,0,3,0);
			tempWarpObject->SetPadding(tmep);
			//tempWarpObject->Padding.Bottom = 3;
			//tempWarpObject->Padding.Top = 3;
			arrWarpButtonWidget.Add(tempWarpObject);
		}
		else
		{
			tempWarpObject = arrWarpButtonWidget[RawNum];
			tempWarpObject->SetVisibility(ESlateVisibility::Visible);
		}
		tempWarpObject->InitButton(Elem);
		tempWarpObject->FOnButtonClicked.BindUObject(this, &UUW_BonefireWarp::OnWarpButtonClicked);
		WarpButtonList->AddChild(tempWarpObject);
		++RawNum;
	}
}

void UUW_BonefireWarp::OnWarpButtonClicked(FName name)
{
	if (GetWorld()->GetGameInstance()->GetSubsystem<UManager_Bonefire>()->GetLastVisitBonefireName() == name) return;
	FTimerHandle TempHandle;
	GetWorld()->GetGameInstance()->GetSubsystem<UManager_UI>()->ClearUI();
	GetWorld()->GetGameInstance()->GetSubsystem<UManager_UI>()->AnnouncementFadeOut("");
	GetWorld()->GetTimerManager().SetTimer(TempHandle, [this ,name]() 
		{
			GetWorld()->GetGameInstance()->GetSubsystem<UManager_Bonefire>()->WarpTargetBonefire(name, PlayerCharacter);
			GetWorld()->GetGameInstance()->GetSubsystem<UManager_UI>()->AnnouncementFadeIn(name.ToString()); 
		},3 ,false);
}

void UUW_BonefireWarpButton::InitButton(FName name)
{
	BoneFireName->SetText(FText::FromString(name.ToString()));
	Name = name;
	Button->OnClicked.AddDynamic(this , &UUW_BonefireWarpButton::OnClicked);
}
void UUW_BonefireWarpButton::OnClicked()
{
	FOnButtonClicked.Execute(Name);
}