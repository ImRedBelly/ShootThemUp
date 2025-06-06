// Shoot Them Up Game. All Rights Reserved.


#include "AI/STUAIController.h"

#include "STURespawnComponent.h"
#include "AI/STUAICharacter.h"
#include "AI/Components/STUAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ASTUAIController::ASTUAIController()
{
    STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("AIPerceptionComponent");
    SetPerceptionComponent(*STUAIPerceptionComponent);

    STURespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent");
    
    bWantsPlayerState = true;
}

void ASTUAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    const auto STUCharacter = Cast<ASTUAICharacter>(InPawn);

    if (IsValid(STUCharacter))
    {
        RunBehaviorTree(STUCharacter->BehaviorTreeAsset);
    }
}

void ASTUAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    const auto AimActor = GetFocusOnActor();
    SetFocus(AimActor);
}

AActor* ASTUAIController::GetFocusOnActor()
{
    if (!GetBlackboardComponent())
    {
        return nullptr;
    }

    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}