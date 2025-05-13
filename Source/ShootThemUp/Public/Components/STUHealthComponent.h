// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "Components/ActorComponent.h"
#include "STUHealthComponent.generated.h"


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


    virtual void BeginPlay() override;

private:
    float Health = 0;

    FTimerHandle HealTimeHandler;

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
        AActor* DamageCauser);

    void HealUpdate();
    void SetHealth(float NewHealth);

};