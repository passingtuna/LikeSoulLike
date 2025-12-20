// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UW_BossHpBar.generated.h"

/**
 * 
 */
UCLASS()
class LIKESOULLIKE_API UUW_BossHpBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HP_GaugeWidget;

	UPROPERTY(meta = (BindWidget))
	UTextBlock * Boss_Name;
	
};
