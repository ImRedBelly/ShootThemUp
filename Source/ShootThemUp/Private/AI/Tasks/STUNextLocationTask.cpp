// Shoot Them Up Game. All Rights Reserved.


#include "AI/Tasks/STUNextLocationTask.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

USTUNextLocationTask::USTUNextLocationTask()
{
    NodeName = "Next Location";
}

EBTNodeResult::Type USTUNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();

    if (!IsValid(Controller) && !IsValid(Blackboard))
    {
        return EBTNodeResult::Failed;
    }

    const auto Pawn = Controller->GetPawn();
    if (!IsValid(Pawn))
    {
        return EBTNodeResult::Failed;
    }

    const auto NavSystem = UNavigationSystemV1::GetCurrent(Pawn);
    if (!IsValid(NavSystem))
    {
        return EBTNodeResult::Failed;
    }

    FNavLocation NavLocation;
    auto Location = Pawn->GetActorLocation();

    if (!SelfCenter)
    {
        auto CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName));
        if (!IsValid(CenterActor))
        {
            return EBTNodeResult::Failed;
        }
        
        Location = CenterActor->GetActorLocation();
    }

    const bool IsFound = NavSystem->GetRandomPointInNavigableRadius(Location, Radius, NavLocation);
    if (!IsFound)
    {
        return EBTNodeResult::Failed;
    }

    Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation);
    return EBTNodeResult::Succeeded;
}