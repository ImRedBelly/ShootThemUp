// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUBasePickup.h"
#include "STUHealthPickup.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUHealthPickup : public ASTUBasePickup
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup", meta=(ClampMin = 1, ClampMax =10))
    float HealthAmount = 100;

private:
    virtual bool GivePickupTo(APawn* PlayerPawn) override;

};