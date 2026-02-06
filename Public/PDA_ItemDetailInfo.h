// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LikeSoulLikeType.h"
#include "PDA_ItemDetailInfo.generated.h"

/**
 * 
 */
class AWeaponDefaultBase;
UCLASS()
class LIKESOULLIKE_API UPDA_ItemDetailInfo : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Item", GetFName());
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemBaseData ItemBaseData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemPresentationData ItemDetailData;
}; 
