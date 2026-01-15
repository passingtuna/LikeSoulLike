// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnhancedInputComponent.h"
#include "LikeSoulLikeType.h"
#include "WeaponDefaultBase.generated.h"

class UDA_ActionData;
class UItemBase;
class ACharacterDefaultBase;
class UAttackDecisionComponent;
class UDA_WeaponDefaultData;
class UManager_Calculate;
class UNiagaraComponent;
UCLASS()
class AWeaponDefaultBase : public AActor
{
	GENERATED_BODY()
	
private:

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle TimerHandle;
	FName NowBuffName;
	TMap<FName, FWeaponMeshInitData> mapWeaponMesh;
	TMap<FName, UNiagaraComponent*> mapNiagara;
	UItemBase* ItemInfo;
	EItemEnchantType EnchantType;
	EActionInputType InputType;
	int32 EventNum;
	ACharacterDefaultBase* OwnerCharacter;
	UAttackDecisionComponent* AttackDecisionComp;
	USceneComponent* LeftHandGripComponet;
	UManager_Calculate* CalculateManager;

	FDamageData			CalBaseDamage;
	FDamageData			BuffDamage;

	float CurrentMotionMutiply;

	FItemData * Itemdata;
public:
	// Sets default values for this actor's properties
	AWeaponDefaultBase();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsTwoHanded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsBuffAble;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* IdleMotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* WalkMotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* RunMotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDA_ActionData* WeakAttackMotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDA_ActionData* StrongAttackMotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDA_ActionData* WeaponActionMotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDA_ActionData* WeaponSkillMotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDA_ActionData* WeaponAvoidMotion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDA_WeaponDefaultData* DADefaultData;
	

	virtual void SettingWeaponData(ACharacterDefaultBase * Character);
	void SetActiveWeapon(bool bIsActive);

	virtual void StrongAttackProcess(ETriggerEvent Trigger);
	virtual void WeakAttackProcess(ETriggerEvent Trigger);
	virtual void WeaponSkillProcess(ETriggerEvent Trigger);
	virtual void WeaponActionProcess(ETriggerEvent Trigger);
	virtual void AvoidProcess(ETriggerEvent Trigger);
	

	void CalculateBaseDamage(ACharacterDefaultBase* Character);
	virtual void ExcueteWeaponEvent(EActionInputType type);
	void IncreaseWeaponEventNum(EActionInputType type) ;
	void ResetWeaponEvent();
	void DecisionAttack(bool isAttacking , ACharacterDefaultBase* Attacker ,UDA_ActionData* Actiondata);

	float GetCurrentMotionMulply() { return CurrentMotionMutiply; };
	void  SetCurrentMotionMulply(float data) {CurrentMotionMutiply = data; };

	void SetItemData(FItemData* dataptr) {Itemdata = dataptr;};
	void SetUpgrade(int32 upgrade) { Itemdata->Upgrade = upgrade; };
	void SetInfusion(EWeaponInfusionType infusion) { Itemdata->Infusion = infusion; };

	FItemData* GetItemData() { return Itemdata; };

	void OnWeaponBuff(FItemAffectData & AffectData);
	void OffWeaponBuff();
};
