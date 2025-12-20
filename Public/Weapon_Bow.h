// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponDefaultBase.h"
#include "Weapon_Bow.generated.h"

/**
 * 
 */
class UAnimInstanceBow;
class UControlRigComponent;
class AProjectile_Default;
class UManager_Projectile;
UCLASS()
class LIKESOULLIKE_API AWeapon_Bow : public AWeaponDefaultBase
{
	GENERATED_BODY()
public:

	virtual void BeginPlay() override;
	virtual void SettingWeaponLocation(ACharacterDefaultBase* Character) override;
	virtual void ExcueteWeaponEvent(EActionInputType type) override;
	virtual void Tick(float DeltaTime) override;
	virtual void StrongAttackProcess( ETriggerEvent Trigger, const FInputActionInstance& instance);
	virtual void WeaponSkillProcess(ETriggerEvent Trigger, const FInputActionInstance& instance);
	virtual void WeaponActionProcess( ETriggerEvent Trigger, const FInputActionInstance& instance);
	virtual void AvoidProcess( ETriggerEvent Trigger, const FInputActionInstance& instance);

	UManager_Projectile* ProjectileManager;
	USkeletalMeshComponent* BowMesh;
	UStaticMeshComponent* ArrowMesh;
	USceneComponent*  StringMidLoc;
	USceneComponent*  ArrowPointLoc;
	USceneComponent* LuanchPos;


	TArray <UStaticMeshComponent *> AdditionalArrow;
	bool IsLoadingBow;
	bool MultiShot;
	bool IsAiming;

	UAnimInstanceBow* BowAnimInstance;
	void ExcuteWeakAttackEvent();
	void ExcuteStrongAttackEvent();
	void ExcuteWeaponActionEvent();

	void ArrowLoaded();
	void ArrowFired();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<AProjectile_Default> ArrowType;
};
