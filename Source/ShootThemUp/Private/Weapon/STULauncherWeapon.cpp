// Shoot Them Up Game. All Rights Reserved.


#include "Weapon/STULauncherWeapon.h"
#include "Weapon/ASTUProjectile.h"

void ASTULauncherWeapon::StartFire()
{
    MakeShoot();
}

void ASTULauncherWeapon::MakeShoot()
{
    const auto Controller = GetPlayerController();
    if (!IsValid(Controller) || IsAmmoEmpty()) return;

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        StopFire();
        return;
    }

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

    FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
    AASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<AASTUProjectile>(ProjectileClass, SpawnTransform);

    if (IsValid(Projectile))
    {
        Projectile->SetOwner(GetOwner());
        Projectile->SetShotDirection(Direction);
        Projectile->FinishSpawning(SpawnTransform);
    }

    DecreaseAmmo();
    SpawnMuzzleFX();
}