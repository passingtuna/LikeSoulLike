// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Manager_UI.generated.h"
/**
 * 
 */
class UUserWidget;
class ACharacterPlayableBase;
class ACharacterNonPlayableBase;
class UDA_UIManagerData;
class AActor;
class UUW_LockONHpBar;
class UUW_CharacterState;
class UUW_BossHpBar;
class UUW_YouDied;
class UUW_StatusWindow;
class UUW_QuickSlot;
class UUW_Inventory;
class UUW_UserMenu;
class ASoulLikeController;
class UUW_StackableUIBase;
UCLASS()
class LIKESOULLIKE_API UManager_UI : public UGameInstanceSubsystem
{
	GENERATED_BODY()
private:
	ACharacterPlayableBase* PlayerCharacter;
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UDA_UIManagerData* GameUIDataAsset;

	TSubclassOf<UUW_CharacterState> CharStateWidgetBP;
	TSubclassOf<UUW_BossHpBar> BossHPWidgetBP;
	TSubclassOf<UUW_YouDied> YouDiedWidgetBp;
	TSubclassOf<UUW_StatusWindow> StatusWidgetBP;
	TSubclassOf<UUW_QuickSlot> QuickSlotWidgetBP;
	TSubclassOf<UUW_Inventory> InventoryWidgetBP;
	TSubclassOf<UUW_UserMenu> UserMenuWidgetBP;

	UPROPERTY()
	UUW_CharacterState* CharStateWidget;
	UPROPERTY()
	UUW_BossHpBar* BossHPWidget;
	UPROPERTY()
	UUW_YouDied* YouDiedWidget;
	UPROPERTY()
	UUW_StatusWindow* StatusWidget;
	UPROPERTY()
	UUW_UserMenu* UserMenuWidget; 


	TArray<UUW_StackableUIBase*> UIStack;
	ASoulLikeController* PlayerController;
	ACharacterPlayableBase* GetPlayerCharacter() { return PlayerCharacter; };

	void TakeControlToUI(UUW_StackableUIBase*FocusWidget);
	void ReturnControlToPlayer();

	void InitUI(ACharacterPlayableBase* Character);

	void SetPlayerHP(float CurrentHP, float MaxHP);
	void SetPlayerSP(float CurrentSP, float MaxSP);
	void SetPlayerMP(float CurrentMP, float MaxMP);

	void SetPlayerMaxHP(float MaxHP);
	void SetPlayerMaxSP(float MaxSP);
	void SetPlayerMaxMP(float MaxMP);

	void SetDeadUI();

	void ShowUIByName(FName uiName);

	void PushUI(UUW_StackableUIBase* NewUI);
	void PopUI();
	UUW_StackableUIBase* GetTopUI();

	void UserMenuKeyAction();
};
