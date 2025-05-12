// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASTUProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class SHOOTTHEMUP_API AASTUProjectile : public AActor
{
    GENERATED_BODY()

public:
    AASTUProjectile();

    void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }

protected:
    UPROPERTY(VisibleAnywhere, Category="Weapon")
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere, Category="Weapon")
    UProjectileMovementComponent* MovementComponent;

    UPROPERTY(EditDefaultsOnly, Category="Weapon")
    float DamageRadius = 200.0f;
    
    UPROPERTY(EditDefaultsOnly, Category="Weapon")
    float DamageAmount = 50.0f;

    UPROPERTY(EditDefaultsOnly, Category="Weapon")
    float LiseSeconds = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category="Weapon")
    bool DoFullDamage = false;
    
    virtual void BeginPlay() override;

private:
    FVector ShotDirection;
    UFUNCTION()
    void Hit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    AController* GetController();
};