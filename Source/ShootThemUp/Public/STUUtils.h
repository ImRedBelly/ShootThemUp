#pragma once
#include "Player/STUPlayerState.h"

class STUUtils
{
public:
    template <typename T>
    static T* GetSTUPlayerComponent(AActor* PlayerPawn)
    {
        if (IsValid(PlayerPawn))
        {
            const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
            return Cast<T>(Component);
        }

        return nullptr;
    }

    static bool AreEnemies(AController* Controller1, AController* Controller2)
    {
        if (IsValid(Controller1) && IsValid(Controller2) && Controller1 != Controller2)
        {

            const auto PlayerState1 = Cast<ASTUPlayerState>(Controller1->PlayerState);
            const auto PlayerState2 = Cast<ASTUPlayerState>(Controller2->PlayerState);

            return IsValid(PlayerState1) && IsValid(PlayerState2) &&
                   PlayerState1->GetTeamID() != PlayerState2->GetTeamID();
        }   

        return false;
    }
};