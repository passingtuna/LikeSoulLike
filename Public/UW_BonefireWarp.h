// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UW_StackableUIBase.h"
#include "UW_BonefireWarp.generated.h"

/**
 * 
 */
class UVerticalBox;
class ACharacterPlayableBase;
UCLASS()
class LIKESOULLIKE_API UUW_BonefireWarp : public UUW_StackableUIBase
{
	GENERATED_BODY()
public:

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* WarpButtonList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUW_BonefireWarpButton> WarpButtonBP;
	ACharacterPlayableBase* PlayerCharacter;

	TArray<UUW_BonefireWarpButton *> arrWarpButtonWidget;

	virtual void InitUI() override;
	void InitBonefireWarpWindow(ACharacterPlayableBase * pc);
	void SettingWarpButton();
	UFUNCTION()
	void OnWarpButtonClicked(FName name);
};

DECLARE_DELEGATE_OneParam(FOnWarpClicked, FName);

class UTextBlock;
class UButton;
UCLASS()
class LIKESOULLIKE_API UUW_BonefireWarpButton : public UUserWidget
{
	GENERATED_BODY()
public:
	FName Name;
	FOnWarpClicked FOnButtonClicked;

	UPROPERTY(meta = (BindWidget))
	UButton* Button;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BoneFireName;
	void InitButton(FName Name);
	UFUNCTION()
	void OnClicked();
};
