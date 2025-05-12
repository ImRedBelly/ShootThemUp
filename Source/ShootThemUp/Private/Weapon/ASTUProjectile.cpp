// Shoot Them Up Game. All Rights Reserved.

#include "Weapon/ASTUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AASTUProjectile::AASTUProjectile()
{
    PrimaryActorTick.bCanEverTick = false;
    CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->InitSphereRadius(5);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
    SetRootComponent(CollisionComponent);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
    MovementComponent->InitialSpeed = 2000.0f;
    MovementComponent->ProjectileGravityScale = 0.0f;
}

void AASTUProjectile::BeginPlay()
{
    Super::BeginPlay();

    check(MovementComponent)
    check(CollisionComponent)

    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;

    CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
    CollisionComponent->OnComponentHit.AddDynamic(this, &AASTUProjectile::Hit);

    SetLifeSpan(LiseSeconds);
}

void AASTUProjectile::Hit(
    UPrimitiveComponent* HitComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (!GetWorld())
    {
        return;
    }

    MovementComponent->StopMovementImmediately();
    UGameplayStatics::ApplyRadialDamage(GetWorld(),
        DamageAmount,
        GetActorLocation(),
        DamageRadius,
        UDamageType::StaticClass(),
        {GetOwner()},
        this,
        GetController(),
        DoFullDamage);

    DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.0f);
    Destroy();
}

AController* AASTUProjectile::GetController()
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return IsValid(Pawn) ? Pawn->GetController() : nullptr;
}