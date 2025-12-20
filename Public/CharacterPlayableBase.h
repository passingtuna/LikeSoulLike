// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "LikeSoulLikeType.h"
#include "CharacterDefaultBase.h"
#include "CharacterPlayableBase.generated.h"	

class AWeaponDefaultBase;
class UItemBase;
class UCameraComponent;
class ASoulLikeController;
UCLASS()
class LIKESOULLIKE_API ACharacterPlayableBase : public ACharacterDefaultBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterPlayableBase();
	FPlayerStatus CurrentStatus;
	int Level;
	int WeaponSlot1WeaponDamage;
	int WeaponSlot2WeaponDamage;
	int WeaponSlot3WeaponDamage;
	bool IsLockOn;
	TArray< FStatusCondition> arrStatusCondition;

	int CurrentWeaponSlot;
	AWeaponDefaultBase* MainWeaponSlot[4];

	TArray< UItemBase*> Inventory;
	TArray< UItemBase*> QuickSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	TSubclassOf<AWeaponDefaultBase> Sword;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	TSubclassOf<AWeaponDefaultBase> Bow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	TSubclassOf<AWeaponDefaultBase> Spear;

	UCameraComponent* CameraComp;
	ACharacterDefaultBase* LockOnTargetChar;
	ASoulLikeController* PlayerController;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float deltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void CalculateResist() override;
	virtual void DiedProcess() override;

	virtual void ModifyCurrentHealth(int param)override;
	virtual void ModifyCurrentStemina(int stemina)override;
	virtual void ModifyCurrentMana(int mana)override;


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
	void Avoid(ETriggerEvent trigger, const FInputActionInstance& Value);
	void UserMenu(ETriggerEvent trigger);

	void LockOn(ETriggerEvent trigger);

	void ExcutingWeaponBaseAction(EActionInputType actionName, ETriggerEvent Trigger, const FInputActionInstance& instance);

	void AddWeaponToMainSlot(TSubclassOf<AWeaponDefaultBase> weapon,int slotNum);
	void EqiupMainSlotWeapon(int slotNum);


	FPlayerStatus GetCurrentStatus() { return CurrentStatus; };

	AWeaponDefaultBase* GetMainWeaponSlot(int slot) 
	{
		if (slot > 2) return nullptr;
		return MainWeaponSlot[slot]; 
	};

};
