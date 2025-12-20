// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile_Default.h"
#include "ProjectileExplosionArrow.generated.h"

/**
 * 
 */
UCLASS()
class LIKESOULLIKE_API AProjectileExplosionArrow : public AProjectile_Default
{
	GENERATED_BODY()
public:
	FDamageData Explosion;
	virtual void BeginPlay() override;
	virtual void OnHitAcotor(const FHitResult& temp) override;
};
