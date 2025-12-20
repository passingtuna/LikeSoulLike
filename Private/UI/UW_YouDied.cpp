// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_YouDied.h"


void UUW_YouDied::NativeConstruct()
{
    Super::NativeConstruct();
    PanelOpacity = 0;
    FadeOutPanel->SetRenderOpacity(0); 
}
void UUW_YouDied::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    if (FadeOutPanel->GetVisibility() == ESlateVisibility::Visible)
    {
        FadeOutPanel->SetRenderOpacity(PanelOpacity);
        PanelOpacity += 0.015f;
    }
}