// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_Announcement.generated.h"

/**
 * 
 */
class UTextBlock;
class UImage;
UCLASS()
class LIKESOULLIKE_API UUW_Announcement : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UImage* FadeOutPanel;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock;
	float PanelOpacity;
	bool IsFadOut;
	bool IsFadIn;
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetFadeOut() { IsFadOut = true;  IsFadIn = false; };
	void SetFadeIn() { IsFadIn = true; IsFadOut = false;};
	void SetText(FString text);

	void ResetAnnouncement();

};
