// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_StackableUIBase.h"
#include "LikeSoulLikeType.h"
#include "UW_Inventory.generated.h"

/**
 * 
 */
class UCanvasPanel;
class UButton;
class UUW_ItemObject;
class UInventoryComponent;
class UManager_ItemInfo;
class UUW_ItemContainer;
class UTextBlock;
UCLASS()
class LIKESOULLIKE_API UUW_Inventory : public UUW_StackableUIBase
{
	GENERATED_BODY()
public:
	UInventoryComponent* InventoryComp;

	UManager_ItemInfo* ItemInfoManager;
	UPROPERTY(meta = (BindWidget))
	UButton* All;
	UPROPERTY(meta = (BindWidget))
	UButton* Equipment;
	UPROPERTY(meta = (BindWidget))
	UButton* Consumable;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameText;
	
	UPROPERTY(meta = (BindWidget))
	UUW_ItemContainer* ItemContainer;

	virtual void InitUI() override;
	void InitInventory(UInventoryComponent* Inventory);

	UFUNCTION()
	void FilterAll();
	UFUNCTION()
	void FilterEquipment();
	UFUNCTION()
	void FilterConsumable();
	
	UFUNCTION()
	void ButtonOnHoverd(int32 Index, EItemButtonActionType ButtenActionType, FItemData ItemData);
	UFUNCTION()
	void ButtonOnUnHoverd(int32 Index, EItemButtonActionType ButtenActionType, FItemData ItemData);
};
