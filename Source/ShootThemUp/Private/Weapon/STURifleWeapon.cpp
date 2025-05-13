// Shoot Them Up Game. All Rights Reserved.

#include "Weapon/STURifleWeapon.h"

#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

ASTURifleWeapon::ASTURifleWeapon()
{
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::StartFire()
{
    InitMuzzleFX();
    GetWorldTimerManager().SetTimer(ShotTimerHandler, this, &ASTURifleWeapon::MakeShoot, TimeBetweenShots, true);
    MakeShoot();
}

void ASTURifleWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandler);
    SetMuzzleVisibility(false);
}

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay();
    check(WeaponFXComponent);
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

        FVector TraceEndFX = TraceEnd;

        if (HitResult.bBlockingHit)
        {
            TraceEndFX = HitResult.ImpactPoint;
            MakeDamage(HitResult);
            WeaponFXComponent->PlayImpactFX(HitResult);
        }

        DecreaseAmmo();
        SpawnTraceFX(GetMuzzleWorldLocation(), TraceEndFX);
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

void ASTURifleWeapon::InitMuzzleFX()
{
    if (!IsValid(MuzzleFXComponent))
    {
        MuzzleFXComponent = SpawnMuzzleFX();
    }

    SetMuzzleVisibility(true);
}

void ASTURifleWeapon::SetMuzzleVisibility(bool Visible)
{
    if (IsValid(MuzzleFXComponent))
    {
        MuzzleFXComponent->SetPaused(!Visible);
        MuzzleFXComponent->SetVisibility(Visible, true);
    }
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;
    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
    TraceStart = ViewLocation;
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

void ASTURifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
    const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
        GetWorld(),
        TraceFX,
        TraceStart);

    if (IsValid(TraceFXComponent))
    {
        TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
    }
}