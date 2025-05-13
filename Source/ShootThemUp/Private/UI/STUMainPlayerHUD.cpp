// Shoot Them Up Game. All Rights Reserved.


#include "UI/STUMainPlayerHUD.h"

#include "STUHealthComponent.h"
#include "STUWeaponComponent.h"
#include "STUUtils.h"


float USTUMainPlayerHUD::GetHealthPercent() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());

    if (IsValid(HealthComponent))
    {
        return HealthComponent->GetHealthPercent();
    }

    return 0;
}

bool USTUMainPlayerHUD::TryGetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());

    if (IsValid(WeaponComponent) && WeaponComponent->TryGetCurrentWeaponUIData(UIData))
    {
        return true;
    }

    return false;
}

bool USTUMainPlayerHUD::TryGetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());

    if (IsValid(WeaponComponent) && WeaponComponent->TryGetCurrentWeaponAmmoData(AmmoData))
    {
        return true;
    }

    return false;
}

bool USTUMainPlayerHUD::IsPlayerAlive() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    return IsValid(HealthComponent) && !HealthComponent->IsDead();
}

bool USTUMainPlayerHUD::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    return IsValid(Controller) && Controller->GetStateName() == NAME_Spectating;
}