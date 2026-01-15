// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LikeSoulLikeType.h"
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
class UUW_Announcement;
class UUW_StatusWindow;
class UUW_QuickSlot;
class UUW_Inventory;
class UUW_UserMenu;
class ASoulLikeController;
class UUW_StackableUIBase;
class UUW_Equipment;
class UUW_Storage;
class UUW_BoneFireMenu;
class UUW_BonefireWarp;
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
	TSubclassOf<UUW_QuickSlot> QuickSlotWidgetBP;

	TSubclassOf<UUW_BossHpBar> BossHPWidgetBP;
	TSubclassOf<UUW_Announcement> AnnouncementWidgetBp;

	TSubclassOf<UUW_UserMenu> UserMenuWidgetBP;
	TSubclassOf<UUW_StatusWindow> StatusWidgetBP;
	TSubclassOf<UUW_Inventory> InventoryWidgetBP;
	TSubclassOf<UUW_Equipment> EquipmentWidgetBP;

	TSubclassOf<UUW_Storage> StorageWidgetBP;
	TSubclassOf<UUW_BoneFireMenu> BonfieMenuWidgetBP;
	TSubclassOf<UUW_BonefireWarp> BonfieWarpMenuWidgetBP;
	TSubclassOf<UUserWidget> AimWidgetBP;
	
	FTimerHandle AnounceTimer;
	

	UPROPERTY()
	UUW_CharacterState* CharStateWidget;
	UPROPERTY()
	UUW_BossHpBar* BossHPWidget;
	UPROPERTY()
	UUW_Announcement* AnnouncementWidget;
	UPROPERTY()
	UUW_StatusWindow* StatusWidget;
	UPROPERTY()
	UUW_UserMenu* UserMenuWidget; 
	UPROPERTY()
	UUW_QuickSlot* QuickSlotWidget;
	UPROPERTY()
	UUW_Inventory* InventoryWidget;
	UPROPERTY()
	UUW_Equipment* EquipmentWidget;
	UPROPERTY()
	UUW_Storage* StorageWidget;
	UPROPERTY()
	UUW_BoneFireMenu* BonfieMenuWidget;
	UPROPERTY()
	UUW_BonefireWarp* BonfieWarpWidget;
	UPROPERTY()
	UUserWidget * AimWidget;

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
	void OpenBonefireMenu(FName Name);

	void ShowBossHPUI(bool On);
	void SetBossHP(float CurrentHP, float MaxHP);
	void SetBossName(FString Name);

	void ShowUIByName(FName uiName);

	void PushUI(UUW_StackableUIBase* NewUI);
	void PopUI();
	void ClearUI();
	UUW_StackableUIBase* GetTopUI();

	void UserMenuKeyAction();

	void ShowAimWidget(bool On);
	
	void ChangeWeaponSlot(FItemData & curWeapon);
	void ChangeQuickSlot(FItemData & curItem);
	void ChangeSoul(int32 soul);
	void ShowGetItemUI(FItemData& curItem, FText Text);

	void AnnouncementText(FString Text);
	void AnnouncementFadeIn(FString Text);
	void AnnouncementFadeOut(FString Text);
	void InvisibleAnnouncement();

};
