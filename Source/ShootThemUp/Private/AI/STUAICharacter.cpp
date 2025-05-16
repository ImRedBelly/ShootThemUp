// Shoot Them Up Game. All Rights Reserved.


#include "AI/STUAICharacter.h"

#include "BrainComponent.h"
#include "STUAIWeaponComponent.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent"))
{
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = ASTUAIController::StaticClass();

    bUseControllerRotationYaw = false;

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0, 200, 0);
    }
}

void ASTUAICharacter::OnDeath()
{
    Super::OnDeath();

    const auto STUController = Cast<AAIController>(Controller);

    if (IsValid(STUController) && IsValid(STUController->BrainComponent))
    {
        STUController->BrainComponent->Cleanup();
    }
}