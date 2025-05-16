// Shoot Them Up Game. All Rights Reserved.

#include "Player/STUBaseCharacter.h"
#include "STUWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");
    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");
}

void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent)
    check(GetCharacterMovement())
    check(GetMesh())

    OnHealthChanged(HealthComponent->GetHealth(), 0);

    HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);
}

void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASTUBaseCharacter::OnStartJump()
{
    Jump();
}

void ASTUBaseCharacter::OnStopJump()
{
    StopJumping();
}

bool ASTUBaseCharacter::IsRunning() const
{
    return false;
}

float ASTUBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero())
    {
        return 0.0f;
    }
    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngelBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);

    const auto Degrees = FMath::RadiansToDegrees(AngelBetween);

    return CrossProduct.IsZero() ? Degrees : FMath::RadiansToDegrees(AngelBetween) * FMath::Sign(CrossProduct.Z);
}

void ASTUBaseCharacter::SetPlayerColor(const FLinearColor& Color)
{
    const auto MaterialInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
    if (IsValid(MaterialInstance))
    {
        MaterialInstance->SetVectorParameterValue(MaterialColorName, Color);
    }
}

void ASTUBaseCharacter::OnDeath()
{
    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(LifeSpanOnDeath);

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
    WeaponComponent->StopFire();

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);

    UE_LOG(LogBaseCharacter, Display, TEXT("Player %s id dead"), *GetName());
}

void ASTUBaseCharacter::OnHealthChanged(float Health, float HealthDelta)
{
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    const auto FallVelocityZ = -GetVelocity().Z;
    UE_LOG(LogBaseCharacter, Display, TEXT("On landed: %f"), FallVelocityZ);

    if (FallVelocityZ < LandedDamageVelocity.X)
    {
        return;
    }

    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);

    UE_LOG(LogBaseCharacter, Display, TEXT("FinalDamage: %f"), FinalDamage);
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}