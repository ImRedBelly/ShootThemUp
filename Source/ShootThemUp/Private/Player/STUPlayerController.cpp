// Shoot Them Up Game. All Rights Reserved.


#include "Player/STUPlayerController.h"

#include "STURespawnComponent.h"

ASTUPlayerController::ASTUPlayerController()
{
    STURespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent");
}