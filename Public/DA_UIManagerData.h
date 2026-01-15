// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UW_LockONHpBar.h"
#include "UW_CharacterState.h"
#include "UW_BossHpBar.h"
#include "Blueprint/UserWidget.h"
#include "UW_Announcement.h"
#include "UW_StatusWindow.h"
#include "UW_Inventory.h"
#include "UW_QuickSlot.h"
#include "UW_UserMenu.h"
#include "UW_Equipment.h"
#include "UW_Storage.h"
#include "UW_BoneFireMenu.h"
#include "UW_BonefireWarp.h"
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
	TSubclassOf<UUW_Announcement> AnnouncementWidgetBp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUW_StatusWindow> StatusWidgetBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUW_Equipment> EquipmentWidgetBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUW_Inventory> InventoryWidgetBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUW_UserMenu> UserMenuWidgetBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUW_Storage> StorageWidgetBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUW_BoneFireMenu> BonfieMenuWidgetBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUW_BonefireWarp> BonfieWarpMenuWidgetBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> AimWidgetBP;
};