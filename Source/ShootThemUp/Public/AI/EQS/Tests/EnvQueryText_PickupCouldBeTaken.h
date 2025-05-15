// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryText_PickupCouldBeTaken.generated.h"


UCLASS()
class SHOOTTHEMUP_API UEnvQueryText_PickupCouldBeTaken : public UEnvQueryTest
{
    GENERATED_BODY()

public:
    UEnvQueryText_PickupCouldBeTaken(const FObjectInitializer& ObjectInitializer);

    virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};