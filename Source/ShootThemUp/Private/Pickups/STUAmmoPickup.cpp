// Shoot Them Up Game. All Rights Reserved.


#include "Pickups/STUAmmoPickup.h"

#include "STUHealthComponent.h"
#include "STUUtils.h"
#include "STUWeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

bool ASTUAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);

    if (!IsValid(HealthComponent) || HealthComponent->IsDead())
    {
        return false;
    }

    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(PlayerPawn);
    if (!IsValid(WeaponComponent))
    {
        return false;
    }

    return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);
}