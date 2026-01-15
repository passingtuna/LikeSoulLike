// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "LikeSoulLikeType.h"
#include "CharacterDefaultBase.h"
#include "GenericTeamAgentInterface.h"
#include "CharacterPlayableBase.generated.h"	

constexpr int32 BaseWeaponSlot = 3;

class AWeaponDefaultBase;
class UItemBase;
class UCameraComponent;
class ASoulLikeController;
class UInventoryComponent;
class UManager_ItemInfo;
class ABoneFire;
class ADropItem;
class UManager_Bonefire;
class USpringArmComponent;
UCLASS()
class LIKESOULLIKE_API ACharacterPlayableBase : public ACharacterDefaultBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties

	virtual FGenericTeamId GetGenericTeamId() const override;


	ACharacterPlayableBase();
	FPlayerStatus CurrentStatus;
	int32 Level;
	int32 WeaponSlot1WeaponDamage;
	int32 WeaponSlot2WeaponDamage;
	int32 WeaponSlot3WeaponDamage;
	TArray< FStatusCondition> arrStatusCondition;

	int32 CurrentWeaponSlotNum;
	int32 CurrentQuickSlotNum;
	AWeaponDefaultBase* MainWeaponSlot[4];
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	TSubclassOf<ADropItem> DropSoulBP;

	UManager_UI* UIManager;
	UManager_ItemInfo* ItemInfoManager;
	UManager_Bonefire* BonefireManager;

	UCameraComponent* CameraComp;
	USpringArmComponent* SpringArmComp;

	ASoulLikeController* PlayerController;
	UInventoryComponent* InventoryComp;
	float LastElapsedInterval;

	float SprintStaminaAcc;

	bool IsAimMode;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float deltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void DiedProcess() override;

	virtual void ModifyCurrentHealth(int32 param)override;
	virtual void ModifyCurrentStemina(int32 stemina)override;
	virtual void ModifyCurrentMana(int32 mana)override;
	void ModifyCurrentSoul(int32 soul);

	ASoulLikeController* GetPlayerController() {return PlayerController;	};
	void HandleInput(FName actionName, ETriggerEvent trigger, const FInputActionInstance& value);

	void UpdateStatus();

	void TryLockOn();
	bool CheckTargetLock(ACharacterDefaultBase * Target, float& ClosestDistSq);
	bool CheckContinueLockOn();
	void ReleaseLockOn();
	void UpdateLockOnRotation(float DeltaTime);


	void SetStatusCondition(FStatusCondition condition);
	void CheckStatusCondition(float deltaTime);
	FStatusCondition* FindStatusByName(FName name);

	void Move(ETriggerEvent trigger, const FInputActionInstance& value);
	void Look(ETriggerEvent trigger, const FInputActionInstance& value);
	void Interact(ETriggerEvent trigger);
	void UseItem(ETriggerEvent trigger);
	void WeaponChange(ETriggerEvent trigger);
	void QuickSlotChange(ETriggerEvent trigger);
	void Avoid(ETriggerEvent trigger, const FInputActionInstance& Value);
	void UserMenu(ETriggerEvent trigger);
	void LockOn(ETriggerEvent trigger);


	void AddWeaponToMainSlot(TSoftClassPtr<AWeaponDefaultBase> weapon, int32 slotNum);
	void EqiupMainSlotWeapon(int32 slotNum);
	void ResetWeaponSlotInfo(int32 slotNum);

	void EqiupQuickSlotItem(int32 slotNum);

	void UpdateCurrentWeaponSlotUI();
	void UpdateCurrentQuickSlotUI();

	void SetAimingMode(bool On);

	void DropSoul();

	void Interact_Item(ADropItem * dropitem);
	void Interact_BoneFire(ABoneFire* dropitem);

	UInventoryComponent* GetInventoryComponent() {return InventoryComp;};
	FPlayerStatus GetCurrentStatus() { return CurrentStatus; };

	AWeaponDefaultBase* GetMainWeaponSlot(int32 slot)
	{
		if (slot > 2) return nullptr;
		return MainWeaponSlot[slot]; 
	};


};
