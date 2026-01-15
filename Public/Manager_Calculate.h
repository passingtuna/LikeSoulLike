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

	FWeaponScaleData CalculateWeaponScale(FWeaponScaleData BasicScale, int32 Upgrade , EWeaponInfusionType WeaponInfusion);
	FWeaponScaleData CalculateWeaponUpgradeScale(int32 Upgrade);
	FWeaponScaleData CalculateWeaponInfusionScale(EWeaponInfusionType Infuse);
	FWeaponScaleData MutiplyScaleDataToScaleData(FWeaponScaleData Elem1, FWeaponScaleData Elem2);

	FDamageData GetWeaponBaseDamage(AWeaponDefaultBase* weapon, int32 Upgrade, EWeaponInfusionType Infusion, FPlayerStatus userStatus);

	FDamageData CalculateWeaponBaseDamage(FDamageData weaponBaseDamage, FPlayerStatus userStatus, FWeaponScaleData weaponScai);

	int32 CalculateMaxHP(int32 Health);
	int32 CalculateMaxStemina(int32 Enducrance);
	int32 CalculateMaxMana(int32 Mentality);
	int32 CalculateMaxToughness(int32 Health);
	int32 CalculateMaxWeight(int32 Stemina);

	FCharacterState CalculateMaxState(FPlayerStatus userStatus);
	FResistData CalculateMaxResist(FPlayerStatus userStatus);
	int32 CalculateResist(int32 Status);

	int32 GetNextRequireSoul(int32 Level);
};
