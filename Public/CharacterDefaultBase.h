// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LikeSoulLikeType.h"
#include "EnhancedInputComponent.h"
#include "GenericTeamAgentInterface.h"
#include "CharacterDefaultBase.generated.h"

class UDA_DefaultMotion;
class UDA_ActionData;
class UAnimInstanceDefaultBase;
class AWeaponDefaultBase;
class UManager_UI;
class UWidgetComponent;
class UManager_Calculate;
class UGrabProcessComponent; 
class UAudioComponent;
class UDA_DefaultSoundData;
UCLASS()
class LIKESOULLIKE_API ACharacterDefaultBase : public ACharacter , public IGenericTeamAgentInterface
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
	bool IsGrabable;		//내가 잡기 가능상태

	bool IsDead;
	bool CanRestoreStemina;

	FVector LastInputDir;
	int32 LastInputSecond;

	FTimerHandle ItemUseTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
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

	UAudioComponent* AudioComp;
	UAudioComponent* AudioEffectComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Init")
	TSoftClassPtr<AWeaponDefaultBase> BaseWeapon;
	UManager_Calculate* CalculatManager;
	AWeaponDefaultBase* CurrentWeapon;
	FTimerHandle SteminaTimer;
	FChargeMotionData ChargeData;
	bool IsLockOn;
	ACharacterDefaultBase* LockOnTargetChar;
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	UGrabProcessComponent* GrapComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	UDA_DefaultMotion* DefaultMotion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	UDA_DefaultSoundData* DefaultSound;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual int32 CalculateFinalDamage(const FDamageData& damageData);

	virtual void ReceiveAttack(ACharacterDefaultBase * Attacker ,const FDamageData damage ,const FVector hitDirection);
	virtual void DiedProcess();
	virtual void AddWeaponToMainSlot(TSoftClassPtr<AWeaponDefaultBase> weapon, int32 slotNum);

	virtual void ModifyCurrentHealth(int32 param);
	virtual void ModifyCurrentStemina(int32 stemina);
	virtual void ModifyCurrentMana(int32 mana);

	void ModifyCurrentToughness(int32 Toughness);
	void SetCurrentToughness(int32 Toughness);

	void SetCrouchState(bool param);
	void ReadyToAction();
	void ActionEnd();
	void SetIsGuarding(bool param) { IsGuarding = param; };
	void SetIsAvoiding(bool param) { IsAvoiding = param; };
	void SetIsParrying(bool param) { IsParrying = param; };
	void SetIsBouncing(bool param) { IsBouncing = param; };
	void SetIsAttacking(bool param);
	void ResetStates();

	virtual void SetIsAttackable(bool param) { IsAttackable = param; };
	virtual void SetIsMoveable(bool param) { IsMoveable = param; };
	virtual void SetIsAvoidable(bool param) { IsAvoidable = param; };
	virtual void SetIsRunable(bool param) { IsRunable = param; };
	virtual void SetIsGrabable(bool param);

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

	void ActionConsumeProcess();
	void ExcutingWeaponBaseAction(EActionInputType actionName, ETriggerEvent Trigger);

	void AvoidMovement(FVector vector ,float Speed);

	void StartLockOnUI();
	void UpdateLockOnUI();
	void EndLockOnUI();

	void SetAnimMontagePlaySpeed(float Temp) { AnimMontagePlaySpeed = Temp; };

	FCharacterState GetCurrentState() {return CurrentState;};
	FResistData GetCurrentResist() { return CurrentResist; };

	bool CheckConsumeableResource(int32 consumeMana);
	FName GetCurrentSection(UAnimMontage * montage);

	bool ExcuteActionMotionByType(UDA_ActionData* actionData, ETriggerEvent Trigger);
	bool ExcuteTypeInstance(UDA_ActionData* actionData, ETriggerEvent Trigger);
	bool ExcuteTypeRelease(UDA_ActionData* actionData, ETriggerEvent Trigger);
	bool ExcuteTypeAvoid(UDA_ActionData* actionData, ETriggerEvent Trigger);
	bool ExcuteTypeCharge(UDA_ActionData* actionData, ETriggerEvent Trigger);
	bool ExcuteTypeHold(UDA_ActionData* actionData, ETriggerEvent Trigger);

	bool CheckHostileChar(ACharacterDefaultBase* TargetChar ,bool IsCheckNeutral);

	void SetStun();
	bool TriedGrab(ACharacterDefaultBase* Attacker );

	bool CheckAndTryGrap();
	
	bool ExcuteItemAffect(FItemAffectData& data);
	void PlaySound(FName SoundName , bool Constance = false);
	void StopSound(bool constance);
};
 