// Shoot Them Up Game. All Rights Reserved.


#include "AI/Components/STUAIPerceptionComponent.h"

#include "AIController.h"
#include "STUHealthComponent.h"
#include "STUUtils.h"
#include "Perception/AISense_Sight.h"

AActor* USTUAIPerceptionComponent::GetClosestEnemy()
{
    TArray<AActor*> PercieveActors;

    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);

    if (PercieveActors.Num() == 0)
    {
        return nullptr;
    }

    const auto Controller = Cast<AAIController>(GetOwner());

    if (!IsValid(Controller))
    {
        return nullptr;
    }

    const auto Pawn = Controller->GetPawn();

    if (!IsValid(Pawn))
    {
        return nullptr;
    }

    float BestDistance = MAX_FLT;
    AActor* BestPawn = nullptr;

    for (auto PercieveActor : PercieveActors)
    {
        const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PercieveActor);

        if (IsValid(HealthComponent) && !HealthComponent->IsDead())
        {
            const auto CurrentDistance = (PercieveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if (CurrentDistance < BestDistance)
            {
                BestDistance = CurrentDistance;
                BestPawn = PercieveActor;
            }
        }
    }

    return BestPawn;
}