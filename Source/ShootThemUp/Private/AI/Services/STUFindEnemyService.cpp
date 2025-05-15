// Shoot Them Up Game. All Rights Reserved.


#include "AI/Services/STUFindEnemyService.h"

#include "AIController.h"
#include "AI/Components/STUAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

USTUFindEnemyService::USTUFindEnemyService()
{
    NodeName = "Find Enemy";
}

void USTUFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();

    if (IsValid(Controller))
    {
        const auto PerceptionComponent = Controller->FindComponentByClass<USTUAIPerceptionComponent>();

        const auto Blackboard = OwnerComp.GetBlackboardComponent();
        if (IsValid(PerceptionComponent) && IsValid(Blackboard))
        {
            Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy());
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}