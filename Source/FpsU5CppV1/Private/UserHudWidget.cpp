// Fill out your copyright notice in the Description page of Project Settings.


#include "UserHudWidget.h"

void UUserHudWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUserHudWidget::SetBarValuePercent(float const value)
{
	HealthValue->SetPercent(value);
}
