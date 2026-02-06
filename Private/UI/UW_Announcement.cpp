// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_Announcement.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UUW_Announcement::NativeConstruct()
{
    Super::NativeConstruct(); 
    ResetAnnouncement();
}
void UUW_Announcement::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    if (IsFadOut)
    {
        FadeOutPanel->SetRenderOpacity(PanelOpacity);
        PanelOpacity += 0.015f;
        //if (PanelOpacity >= 1.0f) IsFadOut = false;
    }

    if (IsFadIn)
    {
        FadeOutPanel->SetRenderOpacity(PanelOpacity);
        PanelOpacity -= 0.015f;
        UE_LOG(LogTemp,Warning,TEXT("페이드인 %f"),PanelOpacity);
        if (PanelOpacity <= 0.0f)
        {
            SetVisibility(ESlateVisibility::Collapsed);
            IsFadIn = false;
        }
    }
}


void UUW_Announcement::SetText(FString string)
{
    TextBlock->SetText(FText::FromString(string));
}


void UUW_Announcement::ResetAnnouncement()
{
    IsFadOut = false;
    IsFadIn = false;
    FadeOutPanel->SetRenderOpacity(0);
    PanelOpacity = 0;
    TextBlock->SetText(FText::FromString(""));
    SetVisibility(ESlateVisibility::Collapsed);
}