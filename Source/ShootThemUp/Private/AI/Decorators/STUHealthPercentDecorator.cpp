// Shoot Them Up Game. All Rights Reserved.


#include "AI/Decorators/STUHealthPercentDecorator.h"

#include "AIController.h"
#include "STUHealthComponent.h"

USTUHealthPercentDecorator::USTUHealthPercentDecorator()
{
    NodeName = "Health Percent";
}

bool USTUHealthPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

    const auto Controller = OwnerComp.GetAIOwner();

    if (IsValid(Controller))
    {
        const auto HealthComponent = Controller->GetPawn()->FindComponentByClass<USTUHealthComponent>();
        if (IsValid(HealthComponent) && !HealthComponent->IsDead())
        {
            return HealthComponent->GetHealthPercent() <= HealthPercent; 
        }
    }

    return false;
}