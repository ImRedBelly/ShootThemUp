// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "Blueprint/UserWidget.h"
#include "STUMainPlayerHUD.generated.h"

class USTUHealthComponent;
class USTUWeaponComponent;

UCLASS()
class SHOOTTHEMUP_API USTUMainPlayerHUD : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="UI")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable, Category="UI")
    bool TryGetCurrentWeaponUIData(FWeaponUIData& UIData) const;

    UFUNCTION(BlueprintCallable, Category="UI")
    bool TryGetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

    UFUNCTION(BlueprintCallable, Category="UI")
    bool IsPlayerAlive() const;

    UFUNCTION(BlueprintCallable, Category="UI")
    bool IsPlayerSpectating() const;

    UFUNCTION(BlueprintImplementableEvent, Category="UI")
    void OnTakeDamage();

    virtual bool Initialize() override;

private:
    UFUNCTION()
    void OnHealthChange(float Health, float HealthDelta);

    void OnNewPawn(APawn* NewPawn);
};