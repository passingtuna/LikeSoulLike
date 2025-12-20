// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "UW_CharacterState.generated.h"

/**
 * 
 */
UCLASS()
class LIKESOULLIKE_API UUW_CharacterState : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HP_GaugeWidget;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* SP_GaugeWidget;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* MP_GaugeWidget;
};