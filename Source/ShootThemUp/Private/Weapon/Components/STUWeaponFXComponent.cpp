// Shoot Them Up Game. All Rights Reserved.

#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

USTUWeaponFXComponent::USTUWeaponFXComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& HitResult)
{
    auto ImpactData = DefaultImpactData;

    if (HitResult.PhysMaterial.IsValid())
    {
        const auto PhysMaterial = HitResult.PhysMaterial.Get();
        if (ImpactDataMap.Contains(PhysMaterial))
        {
            ImpactData = ImpactDataMap[PhysMaterial];
        }
    }

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
        ImpactData.NiagaraEffect,
        HitResult.ImpactPoint,
        HitResult.ImpactNormal.Rotation());

    auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
        ImpactData.DecalData.Material,
        ImpactData.DecalData.Size,
        HitResult.ImpactPoint,
        HitResult.ImpactNormal.Rotation());

    if (IsValid(DecalComponent))
    {
        DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime,
            ImpactData.DecalData.FadeOutTime);
    }
}