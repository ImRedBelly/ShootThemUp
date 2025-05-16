// Shoot Them Up Game. All Rights Reserved.


#include "UI/STUSpectatorWidget.h"

#include "STURespawnComponent.h"

bool USTUSpectatorWidget::GetRespawnTime(int32& CountDownTime) const
{
    const auto RespawnComponent = GetOwningPlayer()->FindComponentByClass<USTURespawnComponent>();
    if (!IsValid(RespawnComponent) || !RespawnComponent->IsRespawnInProgress())
    {
        return false;
    }

    CountDownTime = RespawnComponent->GetRespawnCountDown();
    return true;
}