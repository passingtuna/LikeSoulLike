// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LikeSoulLikeType.h"
#include "DA_ItemInfoManager.generated.h"

/**
 * 
 */
UCLASS()
class LIKESOULLIKE_API UDA_ItemInfoManager : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FIconAtlasData> arrIconAtlas;
};
