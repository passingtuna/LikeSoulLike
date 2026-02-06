// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_StackableUIBase.h"
#include "UW_BoneFireMenu.generated.h"

/**
 * 
 */
class UManager_UI;
class UButton;
class UTextBlock;
class UManager_Bonefire;
UCLASS()
class LIKESOULLIKE_API UUW_BoneFireMenu : public UUW_StackableUIBase
{
	GENERATED_BODY()
public:
	UManager_UI* UIManager;
	UManager_Bonefire* BonefireManager;
	FName BonefireName;

	UPROPERTY(meta = (BindWidget))
	UButton* LevelUp;
	UPROPERTY(meta = (BindWidget))
	UButton* Storage;
	UPROPERTY(meta = (BindWidget))
	UButton* Warp;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BonefireNameText;
	void SetBonefireName(FName name);
	void InitBonefireMenu();
	UFUNCTION()
	void ClickedLevelUpButton();
	UFUNCTION()
	void ClickedWarpButton();
	UFUNCTION()
	void ClickedStorageButton();
};
