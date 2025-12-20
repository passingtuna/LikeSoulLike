// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstanceBow.generated.h"

/**
 * 
 */
UCLASS()
class LIKESOULLIKE_API UAnimInstanceBow : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FTransform BowStringLocation;

	void SetBowStringTransform(FTransform temp) { BowStringLocation = temp; };
};
