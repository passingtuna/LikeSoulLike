// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_StackableUIBase.h"


FReply UUW_StackableUIBase::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) //장비창 닫기 공통 항목
{
    if (InKeyEvent.GetKey() == EKeys::EKeys::One)
    {
        OnRequestClose.ExecuteIfBound();
        return FReply::Handled();
    }
    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}