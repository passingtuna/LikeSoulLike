// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_GaugeWidget.h"

void UUW_GaugeWidget::SetGaugePercent(float Percent)
{
	if (GaugeBar)
	{
		GaugeBar->SetPercent(Percent);
	}
}

void UUW_GaugeWidget::SetGaugeColor(FLinearColor Color)
{
	if (GaugeBar)
	{
		GaugeBar->SetFillColorAndOpacity(Color);
	}
}