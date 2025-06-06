﻿// Shoot Them Up Game. All Rights Reserved.


#include "UI/STUHealthBarWidget.h"

#include "Components/ProgressBar.h"

void USTUHealthBarWidget::SetHealthPercent(float Percent)
{
    if (IsValid(HealthProgressBar))
    {
        const auto HealthBarVisibility = (Percent > PercentVisibilityThreshold || FMath::IsNearlyZero(Percent))
                                             ? ESlateVisibility::Hidden
                                             : ESlateVisibility::Visible;
        HealthProgressBar->SetVisibility(HealthBarVisibility);

        const auto HealthBarColor = Percent > PercentColorThreshold ? GoodColor : BadColor;
        HealthProgressBar->SetFillColorAndOpacity(HealthBarColor);

        HealthProgressBar->SetPercent(Percent);
    }
}