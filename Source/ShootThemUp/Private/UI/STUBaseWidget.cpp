// Shoot Them Up Game. All Rights Reserved.


#include "UI/STUBaseWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void USTUBaseWidget::Show()
{
    if (IsValid(ShowAnimation))
    {
        PlayAnimation(ShowAnimation);
        UGameplayStatics::PlaySound2D(GetWorld(), OpenSound);
    }
}