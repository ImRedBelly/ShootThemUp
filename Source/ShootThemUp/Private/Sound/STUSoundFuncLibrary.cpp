// Shoot Them Up Game. All Rights Reserved.


#include "Sound/STUSoundFuncLibrary.h"

#include "Sound/SoundClass.h"

void USTUSoundFuncLibrary::SetSoundClassVolume(USoundClass* SoundClass, float Volume)
{
    if (IsValid(SoundClass))
    {
        SoundClass->Properties.Volume = FMath::Clamp(Volume, 0, 1);
    }
}

void USTUSoundFuncLibrary::ToggleSoundClassVolume(USoundClass* SoundClass)
{
    if (IsValid(SoundClass))
    {
        const auto NextVolume = SoundClass->Properties.Volume > 0 ? 0 : 1;
        SetSoundClassVolume(SoundClass, NextVolume);
    }
}