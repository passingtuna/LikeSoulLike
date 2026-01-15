// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager_Calculate.h"
#include "CharacterDefaultBase.h"
#include "CharacterPlayableBase.h"
#include "CharacterNonPlayableBase.h"
#include "WeaponDefaultBase.h"
#include "DA_WeaponDefaultData.h"
#include "DA_CalculateManagerData.h"
#include "DA_ActionData.h"


void UManager_Calculate::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	static const FString AssetPath = "/Game/DataAsset/Manager/CalculateManagerDataAsset.CalculateManagerDataAsset";
	DamageCalculateAsset = Cast<UDA_CalculateManagerData>(StaticLoadObject(UDA_CalculateManagerData::StaticClass(), nullptr, *AssetPath));

	if (DamageCalculateAsset)
	{
		InfusionScaleData = DamageCalculateAsset->InfusionScaleData;
		StatusCapCurve = DamageCalculateAsset->StatusCapCurve;
	}
}

FDamageData UManager_Calculate::GetWeaponBaseDamage(AWeaponDefaultBase* weapon, int32 Upgrade, EWeaponInfusionType Infusion, FPlayerStatus userStatus)
{
	FDamageData result;
	if (!weapon) return result;
	FDamageData WeaponBaseDamage = weapon->DADefaultData->DamageData.BaseDamage;
	FWeaponScaleData BasicScale = weapon->DADefaultData->DamageData.BaseScale;
	FWeaponScaleData WeaponScaleResult = CalculateWeaponScale(BasicScale, Upgrade, Infusion);
	result = CalculateWeaponBaseDamage(WeaponBaseDamage, userStatus, WeaponScaleResult);
	return result;
}


FWeaponScaleData UManager_Calculate::CalculateWeaponScale(FWeaponScaleData BasicScale, int32 Upgrade, EWeaponInfusionType WeaponInfusion)
{
	FWeaponScaleData result;
	FWeaponScaleData weaponBaseScale = BasicScale;
	FWeaponScaleData ugradeResultScale = CalculateWeaponUpgradeScale(Upgrade);
	FWeaponScaleData infusionResultScale = CalculateWeaponInfusionScale(WeaponInfusion);
	FWeaponScaleData resultUpgradeXInfusionScale = MutiplyScaleDataToScaleData(ugradeResultScale, infusionResultScale);
	FWeaponScaleData resultBaseXEditedState = MutiplyScaleDataToScaleData(weaponBaseScale, resultUpgradeXInfusionScale);
	result = resultBaseXEditedState;
	return result;
}
FWeaponScaleData UManager_Calculate::CalculateWeaponUpgradeScale(int32 Upgrade)
{
	FWeaponScaleData result;
	if (UCurveFloat** ItemUpgradeScaleCurve = StatusCapCurve.Find("Upgrade_Damage"))
	{
		if((*ItemUpgradeScaleCurve))
		{
			float StatScale = (*ItemUpgradeScaleCurve)->GetFloatValue(Upgrade);
			result.BaseScale = StatScale;
			result.StrScale = StatScale;
			result.DexScale = StatScale;
			result.FaiScale = StatScale;
			result.IntScale = StatScale;
		}
	}
	return result;
}
FWeaponScaleData UManager_Calculate::CalculateWeaponInfusionScale(EWeaponInfusionType Infuse)
{
	FWeaponScaleData result;
	if ((int)Infuse > InfusionScaleData.Num() - 1) return result;
	result = InfusionScaleData[(int)Infuse];
	return result;
}


FWeaponScaleData UManager_Calculate::MutiplyScaleDataToScaleData(FWeaponScaleData Elem1, FWeaponScaleData Elem2)
{
	FWeaponScaleData result;
	result.BaseScale = Elem1.BaseScale * Elem2.BaseScale;
	result.StrScale = Elem1.StrScale * Elem2.StrScale;
	result.DexScale = Elem1.DexScale * Elem2.DexScale;
	result.FaiScale = Elem1.FaiScale * Elem2.FaiScale;
	result.IntScale = Elem1.IntScale * Elem2.IntScale;
	return result;
}

						
FDamageData UManager_Calculate::CalculateWeaponBaseDamage(FDamageData weaponBaseDamage, FPlayerStatus userStatus, FWeaponScaleData weaponScale)
{
	FDamageData result = weaponBaseDamage * (weaponScale.BaseScale); //기본 배율 부터 곱해준다

	float CapReult;
	if (UCurveFloat** TargetCap = StatusCapCurve.Find("Str_Physical"))
	{
		CapReult = (*TargetCap)->GetFloatValue(userStatus.Strong);
		result.PhysicalDamage	+= userStatus.Strong * weaponScale.StrScale;

	}

	if (UCurveFloat** TargetCap = StatusCapCurve.Find("Dex_Physical"))
	{
		CapReult = (*TargetCap)->GetFloatValue(userStatus.Dextery);
		result.PhysicalDamage += userStatus.Dextery * weaponScale.DexScale;

	}
	if (UCurveFloat** TargetCap = StatusCapCurve.Find("Fai_Devine"))
	{
		CapReult = (*TargetCap)->GetFloatValue(userStatus.Faith);
		result.DevineDamage += CapReult * weaponScale.FaiScale;

	}
	if (UCurveFloat** TargetCap = StatusCapCurve.Find("Fai_Lightning"))
	{
		CapReult = (*TargetCap)->GetFloatValue(userStatus.Faith);
		result.FireDamage += CapReult * weaponScale.FaiScale;

	}
	if (UCurveFloat** TargetCap = StatusCapCurve.Find("Int_Magic"))
	{
		CapReult = (*TargetCap)->GetFloatValue(userStatus.Intellegence);
		result.MagicDamage += CapReult * weaponScale.IntScale;

	}
	if (UCurveFloat** TargetCap = StatusCapCurve.Find("Int_Fire"))
	{
		CapReult = (*TargetCap)->GetFloatValue(userStatus.Intellegence);
		result.LightningDamage += CapReult * weaponScale.IntScale;

	}return result;
}
int32 UManager_Calculate::CalculateMaxHP(int32 Health)
{
	int32 result;
	UCurveFloat** TargetCap = StatusCapCurve.Find("Hea_HP");
	result = (*TargetCap)->GetFloatValue(Health);
	return (result * 10);
}
int32 UManager_Calculate::CalculateMaxStemina(int32 Endurance)
{
	int32 result;
	UCurveFloat** TargetCap = StatusCapCurve.Find("End_SP");
	result = (*TargetCap)->GetFloatValue(Endurance);
	return result;

}
int32 UManager_Calculate::CalculateMaxMana(int32 Mentality)
{
	int32 result;
	UCurveFloat** TargetCap = StatusCapCurve.Find("Men_MP");
	result = (*TargetCap)->GetFloatValue(Mentality);
	return result;

}
int32 UManager_Calculate::CalculateMaxToughness(int32 Mentality)
{
	int32 result;
	UCurveFloat** TargetCap = StatusCapCurve.Find("Hea_Toughness");
	result = (*TargetCap)->GetFloatValue(Mentality);
	return result;

}
int32 UManager_Calculate::CalculateMaxWeight(int32 Mentality)
{
	int32 result;
	UCurveFloat** TargetCap = StatusCapCurve.Find("End_Weight");
	result = (*TargetCap)->GetFloatValue(Mentality);
	return result;
}

FCharacterState UManager_Calculate::CalculateMaxState(FPlayerStatus userStatus)
{
	FCharacterState result;
	result.HP = CalculateMaxHP(userStatus.Health);
	result.Mana = CalculateMaxMana(userStatus.Mentality);
	result.Stemina = CalculateMaxStemina(userStatus.Endurance);
	result.Toughness = CalculateMaxToughness(userStatus.Health);
	result.Weight = CalculateMaxWeight(userStatus.Endurance);
	return result;
}

FResistData UManager_Calculate::CalculateMaxResist(FPlayerStatus userStatus)
{
	FResistData result;
	result.PhysicalResist	= CalculateResist(userStatus.Health);
	result.DevineResist		= CalculateResist(userStatus.Mentality);
	result.FireResist		= CalculateResist(userStatus.Endurance);
	result.LightningResist	= CalculateResist(userStatus.Intellegence);
	result.MagicResist		= CalculateResist(userStatus.Faith);
	return result;
}

int32 UManager_Calculate::CalculateResist(int32 Status)
{
	int32 result;
	UCurveFloat** TargetCap = StatusCapCurve.Find("All_Resist");
	result = (*TargetCap)->GetFloatValue(Status);
	return result;
}

int32 UManager_Calculate::GetNextRequireSoul(int32 Level)
{
	int32 result;
	UCurveFloat** TargetCap = StatusCapCurve.Find("LevelRequireSoul");
	result = (*TargetCap)->GetFloatValue(Level);
	return result;
}