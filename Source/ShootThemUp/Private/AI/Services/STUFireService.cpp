// Shoot Them Up Game. All Rights Reserved.


#include "AI/Services/STUFireService.h"

#include "AIController.h"
#include "STUWeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

USTUFireService::USTUFireService()
{
    NodeName = "Fire";
}

void USTUFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();

    const auto HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

    if (IsValid(Controller))
    {
        const auto WeaponComponent = Controller->GetPawn()->GetComponentByClass<USTUWeaponComponent>();
        if (IsValid(WeaponComponent))
        {
            if (HasAim)
            {
                WeaponComponent->StartFire();
            }
            else
            {
                WeaponComponent->StopFire();
            }
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}