// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_DefaultMotion.generated.h"

/**
 * 
 */
UCLASS()
class LIKESOULLIKE_API UDA_DefaultMotion : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* HitMotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* DiedMotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* StunMotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* GrabMotion;
};
