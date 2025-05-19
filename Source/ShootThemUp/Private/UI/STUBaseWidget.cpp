// Shoot Them Up Game. All Rights Reserved.


#include "UI/STUBaseWidget.h"

#include "Animation/WidgetAnimation.h"

void USTUBaseWidget::Show()
{
    if (IsValid(ShowAnimation))
    {
        PlayAnimation(ShowAnimation);
    }
}