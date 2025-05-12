// Shoot Them Up Game. All Rights Reserved.

#include "Weapon/STURifleWeapon.h"

void ASTURifleWeapon::StartFire()
{
    GetWorldTimerManager().SetTimer(ShotTimerHandler, this, &ASTURifleWeapon::MakeShoot, TimeBetweenShots, true);
    MakeShoot();
}

void ASTURifleWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandler);
}

void ASTURifleWeapon::MakeShoot()
{
    if (GetWorld() && !IsAmmoEmpty())
    {
        const auto Controller = GetPlayerController();
        if (!IsValid(Controller)) return;

        FVector TraceStart, TraceEnd;
        if (!GetTraceData(TraceStart, TraceEnd))
        {
            StopFire();
            return;
        }

        FHitResult HitResult;
        MakeHit(HitResult, TraceStart, TraceEnd);

        if (HitResult.bBlockingHit)
        {
            MakeDamage(HitResult);
            DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0.0f, 3.0f);
            DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 24, FColor::Red, false, 5);

        }

        DecreaseAmmo();
    }
    else
    {
        StopFire();
    }
}


void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult)
{
    const auto DamageActor = HitResult.GetActor();
    if (IsValid(DamageActor))
    {
        DamageActor->TakeDamage(DamageAmount, FDamageEvent{}, GetPlayerController(), this);
    }
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;
    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
    TraceStart = ViewLocation;const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}