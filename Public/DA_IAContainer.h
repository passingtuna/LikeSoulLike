// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnhancedInputComponent.h"
#include "DA_IAContainer.generated.h"

/**
 * 
 */
UCLASS()
class LIKESOULLIKE_API UDA_IAContainer : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, UInputAction*> Actions;

	FName FindNameByAction(const UInputAction* action) const
	{
		for (auto & Elem : Actions)
		{
			if (Elem.Value == action)
			{
				return Elem.Key;
			}
		}
		return "null";
	};
};
