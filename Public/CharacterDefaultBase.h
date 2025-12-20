// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LikeSoulLikeType.h"
#include "EnhancedInputComponent.h"
#include "CharacterDefaultBase.generated.h"

class UDA_DefaultMotion;
class UDA_ActionData;
class UAnimInstanceDefaultBase;
class AWeaponDefaultBase;
class UManager_UI;
class UWidgetComponent;
class UManager_Calculate;
UCLASS()
class LIKESOULLIKE_API ACharacterDefaultBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterDefaultBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool IsRunning;
	bool IsGuarding;
	bool IsAvoiding;
	bool IsParrying;
	bool IsBouncing;
	bool IsAttacking;

	bool IsCrouch;

	bool IsMoveable;		//이동 아이템 사용등 비전투 모션 가능
	bool IsAvoidable;	//회피 가능
	bool IsAttackable;  //공격 가능
	bool IsRunable;		//달리기 가능

	bool IsDead;
	bool CanRestoreStemina;

	FVector LastInputDir;
	int LastInputSecond;

	ECharacterCamp CharacterCamp;

	FCharacterState MaxState;
	FCharacterState CurrentState;
	FResistData CurrentResist;

	float AnimMontagePlaySpeed;

	UPROPERTY()
	UDA_ActionData* CurrentPlayActionData;

	UAnimInstanceDefaultBase* AnimInstance;
	UCharacterMovementComponent* CharacterMovement;
	UWidgetComponent* LockOnWidget_HpBar;
	UWidgetComponent* LockOnWidget_Dot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Init")
	TSubclassOf<AWeaponDefaultBase> BaseWeapon;
	UManager_UI* UIManager;
	UManager_Calculate* CalculatManager;

	AWeaponDefaultBase* CurrentWeapon;
	FTimerHandle SteminaTimer;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	UDA_DefaultMotion* DefaultMotion;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual int CalculateFinalDamage(const FDamageData& damageData);

	virtual void CalculateResist();

	virtual void ReceiveAttack(const FDamageData damage ,const FVector hitDirection);
	virtual void DiedProcess();
	virtual void AddWeaponToMainSlot(TSubclassOf<AWeaponDefaultBase> weapon, int slotNum);

	virtual void ModifyCurrentHealth(int param);
	virtual void ModifyCurrentStemina(int stemina);
	virtual void ModifyCurrentMana(int mana);

	void ModifyCurrentToughness(int Toughness);
	void SetCurrentToughness(int Toughness);

	void SetCrouchState(bool param);
	void ReadyToAction();
	void ActionEnd();
	void SetIsGuarding(bool param) { IsGuarding = param; };
	void SetIsAvoiding(bool param) { IsAvoiding = param; };
	void SetIsParrying(bool param) { IsParrying = param; };
	void SetIsBouncing(bool param) { IsBouncing = param; };
	void SetIsAttacking(bool param);

	void SetIsMoveable(bool param) { IsMoveable = param; };
	

	void SetIsAttackable(bool param) { IsAttackable = param; };
	void SetIsRunable(bool param) { IsRunable = param; };
	void SetIsAvoidable(bool param) { IsAvoidable = param; };
	void SetCanRestoreStemina(bool param) { CanRestoreStemina = param; };

	UFUNCTION(BlueprintCallable)
	AWeaponDefaultBase* GetCurrentWeapon() { return CurrentWeapon; };
	FName GetDirectionName(FVector Target);
	FVector GetLastInputDir() { return LastInputDir; };
	UDA_ActionData* GetCurrentPlayActionData(){ return CurrentPlayActionData; };
	void GetCurrentPlayActionData(UDA_ActionData* curAction){ CurrentPlayActionData = curAction; };
	UAnimInstanceDefaultBase* GetAnimInstance() { return AnimInstance; };	
	UCharacterMovementComponent* GetCharacterMovementComponent() { return CharacterMovement; };
	bool GetIsDead() { return IsDead; };

	ECharacterCamp GetCharCamp() { return CharacterCamp; };


	void ActionConsumeProcess();
	bool ExcuteActionMotionByType(UDA_ActionData* actionData, ETriggerEvent Trigger, const FInputActionInstance& instance);

	void AvoidMovement(FVector vector ,float Speed);

	void StartLockOnUI();
	void UpdateLockOnUI();
	void EndLockOnUI();

	void SetAnimMontagePlaySpeed(float Temp) { AnimMontagePlaySpeed = Temp; };

	FCharacterState GetCurrentState() {return CurrentState;};
	FResistData GetCurrentResist() { return CurrentResist; };
};
 