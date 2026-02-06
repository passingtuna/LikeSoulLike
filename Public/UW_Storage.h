// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UW_StackableUIBase.h"
#include "UW_Storage.generated.h"

/**
 * 
 */
class UInventoryComponent;
class UUW_ItemObject;
class UManager_ItemInfo;
class UUW_ItemContainer;
class UButton;
class ACharacterPlayableBase;
class UTextBlock;
UCLASS()
class LIKESOULLIKE_API UUW_Storage : public UUW_StackableUIBase
{
	GENERATED_BODY()
public:
	ACharacterPlayableBase* PlayerChar;
	UInventoryComponent* InventoryComp;
	UManager_ItemInfo* ItemInfoManager;

	UPROPERTY(meta = (BindWidget))
	UUW_ItemContainer* ItemContainer_Inventory;

	UPROPERTY(meta = (BindWidget))
	UUW_ItemContainer* ItemContainer_Storage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameText_Inventory;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameText_Storage;


	UPROPERTY(meta = (BindWidget))
	UButton* All;
	UPROPERTY(meta = (BindWidget))
	UButton* Equipment;
	UPROPERTY(meta = (BindWidget))
	UButton* Consumable;

	virtual void InitUI()override;
	void InitStorage(ACharacterPlayableBase* Character);

	UFUNCTION()
	void FilterAll();
	UFUNCTION()
	void FilterEquipment();
	UFUNCTION()
	void FilterConsumable();


	UFUNCTION()
	void ButtonOnClicked_Storage(int32 Index, EItemButtonActionType ButtenActionType, FItemData ItemData);
	UFUNCTION()
	void ButtonOnHoverd_Storage(int32 Index, EItemButtonActionType ButtenActionType, FItemData ItemData);
	UFUNCTION()
	void ButtonOnUnHoverd_Storage(int32 Index, EItemButtonActionType ButtenActionType, FItemData ItemData);

	UFUNCTION()
	void ButtonOnClicked_Inventory(int32 Index, EItemButtonActionType ButtenActionType, FItemData ItemData);
	UFUNCTION()
	void ButtonOnHoverd_Inventory(int32 Index, EItemButtonActionType ButtenActionType, FItemData ItemData);
	UFUNCTION()
	void ButtonOnUnHoverd_Inventory(int32 Index, EItemButtonActionType ButtenActionType, FItemData ItemData);


};
