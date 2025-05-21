// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUBaseWeapon.h"
#include "STULauncherWeapon.generated.h"

class AASTUProjectile;
class USoundCue;

UCLASS()
class SHOOTTHEMUP_API ASTULauncherWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()

public:
    virtual void StartFire() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    TSubclassOf<AASTUProjectile> ProjectileClass;
 
    UPROPERTY(EditDefaultsOnly, Category="Sound")
    USoundCue* NoAmmoSound;
    
    virtual void MakeShoot() override;

};