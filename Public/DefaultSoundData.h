// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DefaultSoundData.generated.h"

/**
 * 
 */
class USoundBase;
UCLASS()
class LIKESOULLIKE_API UDA_DefaultSoundData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Sound)
	TMap<FName, USoundBase*> SoundEffectMap;
};
