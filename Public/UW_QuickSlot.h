// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_ItemObject.h"
#include "UW_QuickSlot.generated.h"

/**
 * 
 */
class UManager_ItemInfo;
class UTextBlock;
class UImage;
UCLASS()
class LIKESOULLIKE_API UUW_QuickSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	FTimerHandle MassageHandle;
	FTimerHandle WeaponHandle;
	FTimerHandle QuickSlotHandle;

	UPROPERTY(meta = (BindWidget))
	UUW_ItemObject* QuickSlot;
	UPROPERTY(meta = (BindWidget))
	UUW_ItemObject* WeaponSlot;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SoulText;

	UPROPERTY(meta = (BindWidget))
	UImage* GetItemInfo;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* GetItemInfoText;
	UPROPERTY(meta = (BindWidget))
	UUW_ItemObject* GettingItemIcon;
	
	void InitQuickslot();

	void SetWeaponSlot(FItemData& Weaponslot);
	void SetQuickSlot(FItemData& Weaponslot);
	void SetSoul(int32 Soul);

	void VisibleItemGetText(FItemData& curItem, FText Weaponslot);
	UFUNCTION()
	void InvisibleItemGetText();
	UFUNCTION()
	void InvisibleWeaponSlot();
	UFUNCTION()
	void InvisibleQuickSlot();
};
