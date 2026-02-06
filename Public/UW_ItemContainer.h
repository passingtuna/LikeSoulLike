// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LikeSoulLikeType.h"
#include "UW_ItemObject.h"
#include "UW_ItemContainer.generated.h"

/**
 * 
 */
class UCanvasPanel;
class UUW_ItemObject;
class UInventoryComponent;
class UManager_ItemInfo;
class ACharacterPlayableBase;
UCLASS()
class LIKESOULLIKE_API UUW_ItemContainer : public UUserWidget
{
	GENERATED_BODY()
public:
	bool IsEmptyObject;
	int32 Size;
	int32 ColumnsNum;
	FOnItemDeligate ClickedFunc;
	FOnItemDeligate HoverdFunc;
	FOnItemDeligate UnHoverdFunc;
	UManager_ItemInfo* ItemInfoManager;
	EItemFilter FilterType;
	EItemButtonActionType ItemButtenActionType;
	int32 arrItemObjectIndex;
	TArray<UUW_ItemObject*> arrItemObjecWidget;

	void SetGrid(TArray<FItemData>& showItems);
	void ResetGrid();
	void InitItemContainer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUW_ItemObject> ItemObject;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CavasPanel;

	void SetFilter(EItemFilter filterType) { FilterType = filterType; }

	void SetItemButtenActionType(EItemButtonActionType itemAction) { ItemButtenActionType = itemAction; };

	void SetIconSize(int32 size) { Size = size; };
	void SetColumnsNum(int32 columns) { ColumnsNum = columns; };

	UUW_ItemObject* GetCashedItemObject();
};
