﻿// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUBasePickup.h"
#include "STUAmmoPickup.generated.h"

class ASTUBaseWeapon;

UCLASS()
class SHOOTTHEMUP_API ASTUAmmoPickup : public ASTUBasePickup
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup", meta=(ClampMin = 0, ClampMax =10))
    int32 ClipsAmount = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup", meta=(ClampMin = 0, ClampMax =10))
    TSubclassOf<ASTUBaseWeapon> WeaponType;

private:
    virtual bool GivePickupTo(APawn* PlayerPawn) override;


};