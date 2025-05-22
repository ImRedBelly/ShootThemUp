// Shoot Them Up Game. All Rights Reserved.

#include "Weapon/STURifleWeapon.h"

#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ASTURifleWeapon::ASTURifleWeapon()
{
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::StartFire()
{
    InitFX();
    GetWorldTimerManager().SetTimer(ShotTimerHandler, this, &ASTURifleWeapon::MakeShoot, TimeBetweenShots, true);
    MakeShoot();
}

void ASTURifleWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandler);
    SetFXActive(false);
}

void ASTURifleWeapon::Zoom(bool Enable)
{
    const auto Controller = Cast<APlayerController>(GetController());

    if (IsValid(Controller) && IsValid(Controller->PlayerCameraManager))
    {
        if (Enable)
        {
            DefaultCameraFOV = Controller->PlayerCameraManager->GetFOVAngle();
        }

        Controller->PlayerCameraManager->SetFOV(Enable ? FOVZoomAngle : DefaultCameraFOV);
    }
}

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay();
    check(WeaponFXComponent);
}

void ASTURifleWeapon::MakeShoot()
{
    if (!GetWorld() || IsAmmoEmpty())
    {
        StopFire();
        return;
    }

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


void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult)
{
    const auto DamageActor = HitResult.GetActor();
    if (IsValid(DamageActor))
    {
        FPointDamageEvent PointDamageEvent;
        PointDamageEvent.HitInfo = HitResult;
        
        DamageActor->TakeDamage(DamageAmount,PointDamageEvent, GetController(), this);
    }
}

void ASTURifleWeapon::InitFX()
{
    if (!IsValid(MuzzleFXComponent))
    {
        MuzzleFXComponent = SpawnMuzzleFX();
    }
    if (!IsValid(FireAudioComponent))
    {
        FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
    }
    SetFXActive(true);
}

void ASTURifleWeapon::SetFXActive(bool IsActive)
{
    if (IsValid(MuzzleFXComponent))
    {
        MuzzleFXComponent->SetPaused(!IsActive);
        MuzzleFXComponent->SetVisibility(IsActive, true);
    }
    if (IsValid(FireAudioComponent))
    {
        IsActive ? FireAudioComponent->Play() : FireAudioComponent->Stop();
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

AController* ASTURifleWeapon::GetController()
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return IsValid(Pawn) ? Pawn->GetController() : nullptr;
}