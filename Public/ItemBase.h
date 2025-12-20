// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LikeSoulLikeType.h"
#include "ItemBase.generated.h"

/**
 */
UCLASS()
class UItemBase : public UObject
{
	GENERATED_BODY()

	int ID;
	FText Name;
	FText Tooltip;
	EItemType Type;
	bool Stackable;
	int Count;

	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;

	int Upgrade;
};
