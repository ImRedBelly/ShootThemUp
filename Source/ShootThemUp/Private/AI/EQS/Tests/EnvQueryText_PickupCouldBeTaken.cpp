// Shoot Them Up Game. All Rights Reserved.


#include "AI/EQS/Tests/EnvQueryText_PickupCouldBeTaken.h"

#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "Pickups/STUBasePickup.h"

UEnvQueryText_PickupCouldBeTaken::UEnvQueryText_PickupCouldBeTaken(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    Cost = EEnvTestCost::Low;
    ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
    SetWorkOnFloatValues(false);
}

void UEnvQueryText_PickupCouldBeTaken::RunTest(FEnvQueryInstance& QueryInstance) const
{
    UObject* DataOwner = QueryInstance.Owner.Get();
    BoolValue.BindData(DataOwner, QueryInstance.QueryID);

    bool WantsBeTakable = BoolValue.GetValue();

    for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
    {
        AActor* ItemActor = GetItemActor(QueryInstance, It.GetIndex());

        const auto PickActor = Cast<ASTUBasePickup>(ItemActor);
        if (IsValid(PickActor))
        {
            const auto CouldBeTaken = PickActor->CouldBeTaken();
            It.SetScore(TestPurpose, FilterType, CouldBeTaken, WantsBeTakable);
        }
    }
}