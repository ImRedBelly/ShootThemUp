// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class USTUWeaponComponent;
class USTUHealthComponent;
class USoundCue;;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer);

    virtual void Tick(float DeltaTime) override;
    virtual void TornOff() override;
    virtual void Reset() override;

    UFUNCTION(BlueprintCallable, Category="Movement")
    virtual bool IsRunning() const;

    UFUNCTION(BlueprintCallable, Category="Movement")
    float GetMovementDirection() const;

    void SetPlayerColor(const FLinearColor& Color);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    USTUHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    USTUWeaponComponent* WeaponComponent;

    UPROPERTY(EditDefaultsOnly, Category="Animation")
    UAnimMontage* DeathAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category="Damage")
    float LifeSpanOnDeath = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category="Damage")
    FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

    UPROPERTY(EditDefaultsOnly, Category="Damage")
    FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

    UPROPERTY(EditDefaultsOnly, Category="Material")
    FName MaterialColorName = "Paint Color";

    UPROPERTY(EditDefaultsOnly, Category="Sound")
    USoundCue* DeathSound;
    
    virtual void BeginPlay() override;
    virtual void OnDeath();

    void OnStartJump();
    void OnStopJump();
    virtual void OnHealthChanged(float Health, float HealthDelta);

private:
    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);

};