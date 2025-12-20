// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponDefaultBase.h"
#include "Weapon_Spear.generated.h"

/**
 * 
 */
UCLASS()
class LIKESOULLIKE_API AWeapon_Spear : public AWeaponDefaultBase
{
	GENERATED_BODY()

	virtual void AvoidProcess(ETriggerEvent Trigger, const FInputActionInstance& instance) override;
	virtual void WeaponSkillProcess(ETriggerEvent Trigger, const FInputActionInstance& instance) override;
	
};
