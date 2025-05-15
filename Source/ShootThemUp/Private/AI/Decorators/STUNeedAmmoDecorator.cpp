// Shoot Them Up Game. All Rights Reserved.


#include "AI/Decorators/STUNeedAmmoDecorator.h"

#include "AIController.h"
#include "STUWeaponComponent.h"

USTUNeedAmmoDecorator::USTUNeedAmmoDecorator()
{
    NodeName = "Need Ammo";
}

bool USTUNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();

    if (IsValid(Controller))
    {
        const auto WeaponComponent = Controller->GetPawn()->FindComponentByClass<USTUWeaponComponent>();
        if (IsValid(WeaponComponent))
        {
            return WeaponComponent->NeedAmmo(WeaponType);
        }
    }

    return false;
}