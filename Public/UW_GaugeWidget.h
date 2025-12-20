// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "UW_GaugeWidget.generated.h"

/**
 * 
 */
UCLASS()
class LIKESOULLIKE_API UUW_GaugeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public :
	UPROPERTY(meta = (BindWidget))
	UProgressBar* GaugeBar;
	void SetGaugePercent(float Percent);
	void SetGaugeColor(FLinearColor Color);
};
