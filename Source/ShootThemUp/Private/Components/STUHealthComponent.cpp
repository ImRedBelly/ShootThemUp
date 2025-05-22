// Shoot Them Up Game. All Rights Reserved.

#include "Components/STUHealthComponent.h"

#include "GameFramework/Character.h"
#include "Perception/AISense_Damage.h"
#include "ShootThemUp/STUGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

USTUHealthComponent::USTUHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
    if (IsDead() || IsHealthFull())
    {
        return false;
    }

    SetHealth(Health + HealthAmount);
    return true;
}

bool USTUHealthComponent::IsHealthFull() const
{
    return FMath::IsNearlyEqual(Health, MaxHealth);
}

void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxHealth > 0);

    SetHealth(MaxHealth);

    AActor* ComponentOwner = GetOwner();

    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
        ComponentOwner->OnTakePointDamage.AddDynamic(this, &USTUHealthComponent::OnTakePointDamage);
        ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &USTUHealthComponent::OnTakeRadialDamage);
    }
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
    AActor* DamageCauser)
{
    UE_LOG(LogHealthComponent, Display, TEXT("On any damage: %f"), Damage);
}

void USTUHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
    UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
    const auto FinalDamage = Damage * GetPointDamageModifier(DamagedActor, BoneName);
    UE_LOG(LogHealthComponent, Display, TEXT("On point damage: %f, final damage: %f, bone: %s"), Damage, FinalDamage,
        *BoneName.ToString());
    ApplyDamage(FinalDamage, InstigatedBy);
}

void USTUHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin,
    const FHitResult& HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
    UE_LOG(LogHealthComponent, Display, TEXT("On radial damage: %f"), Damage);
    ApplyDamage(Damage, InstigatedBy);
}

void USTUHealthComponent::HealUpdate()
{
    SetHealth(Health + HealModifier);

    if (IsHealthFull() && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimeHandler);
    }
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
    const auto NextHealth = FMath::Clamp(NewHealth, 0, MaxHealth);
    const auto HealthDelta = NextHealth - Health;

    Health = NextHealth;
    OnHealthChanged.Broadcast(Health, HealthDelta);
}

void USTUHealthComponent::PlayCameraShake()
{
    if (IsDead())
    {
        return;
    }

    const auto Player = Cast<APawn>(GetOwner());
    if (IsValid(Player))
    {
        const auto Controller = Player->GetController<APlayerController>();
        if (IsValid(Controller) && IsValid(Controller->PlayerCameraManager))
        {
            Controller->PlayerCameraManager->StartCameraShake(CameraShakeClass);
        }
    }
}

void USTUHealthComponent::Killed(AController* KillerController)
{
    if (!GetWorld()) return;
    const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());

    if (IsValid(GameMode))
    {
        const auto Player = Cast<APawn>(GetOwner());
        const auto VictimController = IsValid(Player) ? Player->Controller : nullptr;
        GameMode->Killed(KillerController, VictimController);
    }
}


void USTUHealthComponent::ApplyDamage(float Damage, AController* InstigatedBy)
{
    if (Damage <= 0 || IsDead() || !GetWorld())
    {
        return;
    }

    SetHealth(Health - Damage);

    GetWorld()->GetTimerManager().ClearTimer(HealTimeHandler);

    if (IsDead())
    {
        Killed(InstigatedBy);
        OnDeath.Broadcast();
    }
    else if (AutoHeal)
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimeHandler, this, &USTUHealthComponent::HealUpdate, HealUpdateTime,
            true, HealDelay);
    }

    PlayCameraShake();
    ReportDamage(Damage, InstigatedBy);
}

float USTUHealthComponent::GetPointDamageModifier(AActor* DamageActor, const FName& BoneName)
{
    const auto Character = Cast<ACharacter>(DamageActor);
    if (IsValid(Character) &&
        Character->GetMesh() &&
        Character->GetMesh()->GetBodyInstance(BoneName))
    {
        const auto PhysicsMaterial = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
        if (IsValid(PhysicsMaterial) && DamageModifiers.Contains(PhysicsMaterial))
        {
            return DamageModifiers[PhysicsMaterial];
        }
    }
    return 1.0f;
}

void USTUHealthComponent::ReportDamage(float Damage, AController* InstigatedBy)
{
    if (GetOwner() &&
        InstigatedBy &&
        InstigatedBy->GetPawn())
    {
        UAISense_Damage::ReportDamageEvent(GetWorld(),
            GetOwner(),
            InstigatedBy->GetPawn(),
            Damage,
            InstigatedBy->GetPawn()->GetActorLocation(),
            GetOwner()->GetActorLocation());
    }
}