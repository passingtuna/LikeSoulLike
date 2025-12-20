// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Manager_Projectile.generated.h"

/**
 * 
 */
class AProjectile_Default;
class UDA_ProjectileManagerData;
UCLASS()
class LIKESOULLIKE_API UManager_Projectile : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	void Initialize(FSubsystemCollectionBase& Collection);
	

	TArray<AProjectile_Default*> ProjectilePool;
	UDA_ProjectileManagerData* ProjectileDataAsset;

	TMap<FName, TSubclassOf<AProjectile_Default>> mapResistProjectile;
	TMap<FName, TArray<AProjectile_Default*>> mapProjectilePool;

	AProjectile_Default* GetProjectile(FName ProJectileNmae);
	void CheckProjectile();
};
