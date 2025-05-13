// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

class USTUWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()

public:
    ASTURifleWeapon();
    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
    float TimeBetweenShots = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
    double BulletSpread = 1.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
    float DamageAmount = 10;

    UPROPERTY(EditDefaultsOnly, Category="VFX")
    UNiagaraSystem* TraceFX;

    UPROPERTY(EditDefaultsOnly, Category="VFX")
    FString TraceTargetName = "TraceTarget";
    
    UPROPERTY(VisibleAnywhere, Category="VFX")
    USTUWeaponFXComponent* WeaponFXComponent;

    virtual void BeginPlay() override;
    virtual void MakeShoot() override;

    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

private:
    FTimerHandle ShotTimerHandler;

    UPROPERTY()
    UNiagaraComponent* MuzzleFXComponent;

    void MakeDamage(const FHitResult& HitResult);
    void InitMuzzleFX();
    void SetMuzzleVisibility(bool Visible);
    void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);
};