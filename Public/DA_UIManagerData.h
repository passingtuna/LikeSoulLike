// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UW_LockONHpBar.h"
#include "UW_CharacterState.h"
#include "UW_BossHpBar.h"
#include "Blueprint/UserWidget.h"
#include "UW_YouDied.h"
#include "UW_StatusWindow.h"
#include "UW_Inventory.h"
#include "UW_QuickSlot.h"
#include "UW_UserMenu.h"
#include "UW_Equipment.h"
#include "DA_UIManagerData.generated.h"

/**
 * 
 */
UCLASS()
class LIKESOULLIKE_API UDA_UIManagerData : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUW_CharacterState> CharStateWidgetBp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUW_QuickSlot> QuickSlotWidgetBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUW_BossHpBar> BossHPWidgetBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUW_YouDied> YouDiedWidgetBp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUW_StatusWindow> StatusWidgetBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUW_Equipment> EquipmentWidgetBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUW_Inventory> InventoryWidgetBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUW_UserMenu> UserMenuWidgetBP;
};