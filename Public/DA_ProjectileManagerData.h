// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_ProjectileManagerData.generated.h"

/**
 * 
 */
class AProjectile_Default;
UCLASS()
class LIKESOULLIKE_API UDA_ProjectileManagerData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, TSubclassOf<AProjectile_Default>> mapResistProjectile;

};
