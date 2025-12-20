// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "UW_LockONHpBar.generated.h"

/**
 * 
 */
UCLASS()
class LIKESOULLIKE_API UUW_LockONHpBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HP_GaugeWidget;
	AActor* TargetChar;

	void SetTartgetChar(AActor* temp) { TargetChar = temp; };
};
