// Shoot Them Up Game. All Rights Reserved.


#include "AI/Services/STUChangeWeaponService.h"

#include "AIController.h"
#include "STUWeaponComponent.h"

USTUChangeWeaponService::USTUChangeWeaponService()
{
    NodeName = "Change Weapon";
}

void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();

    if (IsValid(Controller))
    {
        const auto WeaponController = Controller->GetPawn()->GetComponentByClass<USTUWeaponComponent>();

        if (IsValid(WeaponController) && Probability > 0 && FMath::FRand() <= Probability)
        {
            WeaponController->NextWeapon();
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}