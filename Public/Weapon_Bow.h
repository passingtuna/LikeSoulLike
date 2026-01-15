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
class UManager_Projectile;// Forward
class USplineComponent;
class USplineMeshComponent;
UCLASS()
class LIKESOULLIKE_API AWeapon_Bow : public AWeaponDefaultBase
{
	GENERATED_BODY()
public:

	virtual void BeginPlay() override;
	virtual void SettingWeaponData(ACharacterDefaultBase* Character) override;
	virtual void ExcueteWeaponEvent(EActionInputType type) override;
	virtual void Tick(float DeltaTime) override;
	virtual void StrongAttackProcess( ETriggerEvent Trigger)override;
	virtual void WeaponSkillProcess(ETriggerEvent Trigger)override;
	virtual void WeaponActionProcess( ETriggerEvent Trigger)override;
	virtual void AvoidProcess( ETriggerEvent Trigger)override;

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
	void ArrowLoaded();
	void ArrowFired();

	void CalculatePredictProjectile();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<AProjectile_Default> ArrowType;



	FTimerHandle AimDrawTimer;
	USplineComponent* SplineComp;
	TArray<USplineMeshComponent*> SplineMeshThrows;
	UPROPERTY(EditDefaultsOnly, Category = "Trajectory")
	UStaticMesh* TrajectoryMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Trajectory")
	UMaterialInterface* TrajectoryMaterial;
	UPROPERTY(EditDefaultsOnly, Category = "Trajectory")
	float TrajectoryRadius = 5.f;
	UPROPERTY(EditDefaultsOnly, Category = "Trajectory")
	float TrajectorySimTime = 3.f;
};
