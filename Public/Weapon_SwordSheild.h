// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponDefaultBase.h"
#include "Weapon_SwordSheild.generated.h"

/**
 * 
 */
UCLASS()
class LIKESOULLIKE_API AWeapon_SwordSheild : public AWeaponDefaultBase
{
	GENERATED_BODY()
	
public:

	virtual void ExcueteWeaponEvent(EActionInputType type) override;
	virtual void StrongAttackProcess(ETriggerEvent Trigger, const FInputActionInstance& instance)override;
};
