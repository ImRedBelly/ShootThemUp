﻿// Shoot Them Up Game. All Rights Reserved.


#include "STUAIWeaponComponent.h"


void USTUAIWeaponComponent::StartFire()
{
    if (!CanFire())
    {
        return;
    }

    if (CurrentWeapon->IsAmmoEmpty())
    {
        NextWeapon();
    }
    else
    {
        CurrentWeapon->StartFire();
    }
}

void USTUAIWeaponComponent::NextWeapon()
{
    if (!CanEquip())
    {
        return;
    }

    int32 NextIndex = (CurrentWeaponIndex + 1) % Weapons.Num();

    while (NextIndex != CurrentWeaponIndex)
    {
        if (!Weapons[NextIndex]->IsAmmoEmpty())
        {
            break;
        }

        NextIndex = (NextIndex + 1) % Weapons.Num();
    }
    if (NextIndex != CurrentWeaponIndex)
    {
        CurrentWeaponIndex = NextIndex;
        EquipWeapon(CurrentWeaponIndex);
    }
}