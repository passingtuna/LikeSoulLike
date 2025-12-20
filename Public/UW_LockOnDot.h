// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "UW_LockOnDot.generated.h"

/**
 *
 */
UCLASS()
class LIKESOULLIKE_API UUW_LockOnDot : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UImage* LockOnDot;
};
