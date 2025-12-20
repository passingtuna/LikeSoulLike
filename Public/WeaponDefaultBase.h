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
UCLASS()
class AWeaponDefaultBase : public AActor
{
	GENERATED_BODY()
	
private:

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	TMap<FName, WeaponMeshInitData> mapWeaponMesh;
	UItemBase* ItemInfo;
	EItemEnchantType EnchantType;
	EActionInputType InputType;
	int EventNum;
	ACharacterDefaultBase* OwnerCharacter;
	int StrongCorrection;
	int DexteryCorrection;
	int IntellegenceCorrection;
	int FaithCorrection;
	int BaseDamage;
	UAttackDecisionComponent* AttackDecisionComp;
	USceneComponent* LeftHandGripComponet;
	UManager_Calculate* CalculatManager;

	float CurrentMotionMutiply;
	int ChargeStep;
	int Upgrade;
	EWeaponInfusionType InfusionType;
public:
	// Sets default values for this actor's properties
	AWeaponDefaultBase();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsTwoHanded;

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
	

	virtual void SettingWeaponLocation(ACharacterDefaultBase * Character);
	void SetActiveWeapon(bool bIsActive);

	virtual void StrongAttackProcess( ETriggerEvent Trigger, const FInputActionInstance& instance);
	virtual void WeakAttackProcess( ETriggerEvent Trigger, const FInputActionInstance& instance);
	virtual void WeaponSkillProcess( ETriggerEvent Trigger, const FInputActionInstance& instance);
	virtual void WeaponActionProcess(ETriggerEvent Trigger, const FInputActionInstance& instance);
	virtual void AvoidProcess( ETriggerEvent Trigger, const FInputActionInstance& instance);

	virtual void ExcueteWeaponEvent(EActionInputType type);
	void IncreaseWeaponEventNum(EActionInputType type) ;
	void ResetWeaponEvent();
	void SetIsAttacking(bool isAttacking , FDamageData* DataInfo , ACharacterDefaultBase* Attacker);

	float GetCurrentMotionMulply() { return CurrentMotionMutiply; };

	void SetUpgrade(int upgrade) { Upgrade = upgrade; };
	void SetInfusion(int infusion) { InfusionType = (EWeaponInfusionType)infusion; };

	int GetUpgrade() { return Upgrade; };
	EWeaponInfusionType GetInfusionType() { return InfusionType; };
};
