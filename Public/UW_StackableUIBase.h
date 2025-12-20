// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_StackableUIBase.generated.h"

/**
 * 
 */
DECLARE_DELEGATE(FOnRequestClose);

UCLASS()
class LIKESOULLIKE_API UUW_StackableUIBase : public UUserWidget
{
	GENERATED_BODY()
public:
	FOnRequestClose OnRequestClose;
	virtual void InitUI() {};

protected:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
