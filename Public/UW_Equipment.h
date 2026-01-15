// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_StackableUIBase.h"
#include "LikeSoulLikeType.h"
#include "UW_ItemObject.h"
#include "UW_Equipment.generated.h"

/**
 * 
 */
class UInventoryComponent;
class UUW_ItemObject;
class UManager_ItemInfo;
class UCanvasPanel;
class ACharacterPlayableBase;
class UUW_ItemContainer;
UCLASS()
class LIKESOULLIKE_API UUW_Equipment : public UUW_StackableUIBase
{
	GENERATED_BODY()
public:
	ACharacterPlayableBase* PlayerChar;
	UInventoryComponent* InventoryComp; 
	UManager_ItemInfo* ItemInfoManager;

	int32 SelectedIndex;
	EItemButtonActionType SelectedSlot;
	FOnItemDeligate Func;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUW_ItemObject> ItemObject;

	UPROPERTY(meta = (BindWidget))
	UUW_ItemObject* QuickSlot1;
	UPROPERTY(meta = (BindWidget))
	UUW_ItemObject* QuickSlot2;
	UPROPERTY(meta = (BindWidget))
	UUW_ItemObject* QuickSlot3;

	UPROPERTY(meta = (BindWidget))
	UUW_ItemObject* WeaponSlot1;
	UPROPERTY(meta = (BindWidget))
	UUW_ItemObject* WeaponSlot2;
	UPROPERTY(meta = (BindWidget))
	UUW_ItemObject* WeaponSlot3;

	UPROPERTY(meta = (BindWidget))
	UUW_ItemContainer* ItemContainer;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameText;

	void InitEquipment(ACharacterPlayableBase* PlayerChar);
	virtual void InitUI() override;
	//void SetGrid();
	//void ResetGrid();

	UFUNCTION()
	void ButtonOnClicked(int32 Index , EItemButtonActionType ButtenActionType ,FItemData ItemData);

	UFUNCTION()
	void ButtonOnHoverd(int32 Index, EItemButtonActionType ButtenActionType, FItemData ItemData);

	UFUNCTION()
	void ButtonOnUnHoverd(int32 Index, EItemButtonActionType ButtenActionType, FItemData ItemData);

	void EquipItem(int32 Index, FItemData ItemData);
;
};
