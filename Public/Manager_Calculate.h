// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LikeSoulLikeType.h"
#include "Manager_Calculate.generated.h"

/**
 * 
 */
class ACharacterDefaultBase;
class ACharacterPlayableBase;
class ACharacterNonPlayableBase;
class AWeaponDefaultBase;
class UDA_CalculateManagerData;
UCLASS()
class LIKESOULLIKE_API UManager_Calculate : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UDA_CalculateManagerData* DamageCalculateAsset;

	TArray<FWeaponScaleData> InfusionScaleData;
	TMap<FName, UCurveFloat*> StatusCapCurve;

	void Initialize(FSubsystemCollectionBase& Collection);
	FDamageData CaculateAttackDamage(ACharacterDefaultBase* Attacker);

	FDamageData CaculatePlayableDamage(ACharacterPlayableBase* Attacker);
	FDamageData CaculateNonePlayableDamage(ACharacterNonPlayableBase* Attacker);

	FWeaponScaleData CalculateWeaponScale(FWeaponScaleData BasicScale, int Upgrade , EWeaponInfusionType WeaponInfusion);
	FWeaponScaleData CalculateWeaponUpgradeScale(int Upgrade);
	FWeaponScaleData CalculateWeaponInfusionScale(EWeaponInfusionType Infuse);
	FWeaponScaleData MutiplyScaleDataToScaleData(FWeaponScaleData Elem1, FWeaponScaleData Elem2);

	FDamageData GetWeaponBaseDamage(AWeaponDefaultBase* weapon, int Upgrade, EWeaponInfusionType Infusion, FPlayerStatus userStatus);

	FDamageData CalculateWeaponBaseDamage(FDamageData weaponBaseDamage, FPlayerStatus userStatus, FWeaponScaleData weaponScai);

	int CalculateMaxHP(int Health);
	int CalculateMaxStemina(int Enducrance);
	int CalculateMaxMana(int Mentality); 
	int CalculateMaxToughness(int Health);
	int CalculateMaxWeight(int Stemina);

	FCharacterState CalculateMaxState(FPlayerStatus userStatus);
	FResistData CalculateMaxResist(FPlayerStatus userStatus);
	int CalculateResist(int Status);

	int GetNextRequireSoul(int Level);
};
