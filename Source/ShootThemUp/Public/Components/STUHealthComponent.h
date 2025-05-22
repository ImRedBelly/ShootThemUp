// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "Components/ActorComponent.h"
#include "Pickups/STUHealthPickup.h"
#include "STUHealthComponent.generated.h"

class UPhysicalMaterial;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTUHealthComponent();

    float GetHealth() const { return Health; }

    UFUNCTION(BlueprintCallable, Category="Health")
    bool IsDead() const { return FMath::IsNearlyZero(Health, 0); }

    UFUNCTION(BlueprintCallable, Category="Health")
    float GetHealthPercent() const { return Health / MaxHealth; }

    bool TryToAddHealth(float HealthAmount);
    bool IsHealthFull() const;

    FOnDeathSignature OnDeath;
    FOnHealthChangedSignature OnHealthChanged;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Health", meta=(ClampMin = 0, ClampMax = 100))
    float MaxHealth = 100;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Health")
    bool AutoHeal = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Health", meta=(EditCondition= "AutoHeal"))
    float HealUpdateTime = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Health", meta=(EditCondition= "AutoHeal"))
    float HealDelay = 3;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Health", meta=(EditCondition= "AutoHeal"))
    float HealModifier = 5;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Health", meta=(EditCondition= "VFX"))
    TSubclassOf<UCameraShakeBase> CameraShakeClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Health", meta=(EditCondition= "AutoHeal"))
    TMap<UPhysicalMaterial*, float> DamageModifiers;

    virtual void BeginPlay() override;

private:
    float Health = 0;

    FTimerHandle HealTimeHandler;

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
        AActor* DamageCauser);

    UFUNCTION()
    void OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
        UPrimitiveComponent* FHitComponent, FName BoneName,
        FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser);

    UFUNCTION()
    void OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, const FHitResult& HitInfo,
        AController* InstigatedBy,
        AActor* DamageCauser);

    void HealUpdate();
    void SetHealth(float NewHealth);

    void PlayCameraShake();
    void Killed(AController* KillerController);

    void ApplyDamage(float Damage, AController* InstigatedBy);
    float GetPointDamageModifier(AActor* DamageActor, const FName& BoneName);
    void ReportDamage(float Damage, AController* InstigatedBy);
};