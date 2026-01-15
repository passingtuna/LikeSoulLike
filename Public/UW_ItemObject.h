// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LikeSoulLikeType.h"
#include "UW_ItemObject.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_ThreeParams(FOnItemDeligate,int32, EItemButtonActionType ,FItemData);

class UImage;
class UButton;
class UTextBlock;
class UUW_ItemObject;
UCLASS()
class LIKESOULLIKE_API UUW_ItemObject : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage;
	UPROPERTY(meta = (BindWidget))
	UButton* Button;
	UPROPERTY(meta = (BindWidget))
	UImage* CountBox;
	UPROPERTY(meta = (BindWidget))
	UTextBlock * Count;

	UPROPERTY(meta = (BindWidget))
	UImage* EquipBox;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Equip;

	FOnItemDeligate OnItemClicked;
	FOnItemDeligate OnItemHovered;
	FOnItemDeligate OnItemUnHovered;

	FItemData ItemData;
	int32 Index;
	EItemButtonActionType ButtenActionType;
	void SetButtenIndexType(int32 index, EItemButtonActionType buttenActionType) { Index = index;  ButtenActionType = buttenActionType; };
	void SetItemInfo(FItemData itemData);
	void InitItemObject();

	UFUNCTION()
	void OnClickedItem();

	UFUNCTION()
	void OnHoveredItem();
	UFUNCTION()
	void OnUnhoveredItem();
};
