#pragma once

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
};