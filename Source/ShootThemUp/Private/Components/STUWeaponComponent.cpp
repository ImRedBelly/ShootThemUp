// Shoot Them Up Game. All Rights Reserved.

#include "STUWeaponComponent.h"

#include "Animations/AnimUtils.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/STUReloadFinishedAnimNotify.h"
#include "GameFramework/Character.h"
#include "Weapon/STUBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

constexpr static int32 WeaponNum = 2;

USTUWeaponComponent::USTUWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

}

void USTUWeaponComponent::StartFire()
{
    if (CanFire())
    {
        CurrentWeapon->StartFire();
    }
}

void USTUWeaponComponent::StopFire()
{
    if (IsValid(CurrentWeapon))
    {
        CurrentWeapon->StopFire();
    }
}

void USTUWeaponComponent::NextWeapon()
{
    if (CanEquip())
    {
        CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
        EquipWeapon(CurrentWeaponIndex);
    }
}

void USTUWeaponComponent::Reload()
{
    ChangeClip();
}

bool USTUWeaponComponent::TryGetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    if (IsValid(CurrentWeapon))
    {
        UIData = CurrentWeapon->GetUIData();
        return true;
    }

    return false;
}

bool USTUWeaponComponent::TryGetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    if (IsValid(CurrentWeapon))
    {
        AmmoData = CurrentWeapon->GetAmmoData();
        return true;
    }

    return false;
}

bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount)
{
    for (auto Weapon : Weapons)
    {
        if (IsValid(Weapon) && Weapon->IsA(WeaponType))
        {
            return Weapon->TryToAddAmmo(ClipsAmount);
        }
    }
    return false;
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    checkf(WeaponData.Num() == WeaponNum, TEXT("Our character can hold only %i weapon items"), WeaponNum);

    CurrentWeaponIndex = 0;

    InitAnimations();
    SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon = nullptr;
    for (auto Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();

    }
    Weapons.Empty();

    Super::EndPlay(EndPlayReason);
}


void USTUWeaponComponent::SpawnWeapons()
{
    if (GetWorld() && !WeaponData.IsEmpty())
    {

        ACharacter* Character = Cast<ACharacter>(GetOwner());
        if (IsValid(Character))
        {

            for (auto OneWeaponData : WeaponData)
            {
                auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);

                if (Weapon)
                {
                    Weapon->SetOwner(GetOwner());
                    Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnEmptyClip);
                    Weapons.Add(Weapon);
                    AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
                }
            }
        }
    }
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
    if (IsValid(Weapon) && IsValid(SceneComponent))
    {
        FAttachmentTransformRules AttachentRules(EAttachmentRule::SnapToTarget, false);
        Weapon->AttachToComponent(SceneComponent, AttachentRules, SocketName);
    }
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    if (WeaponIndex < 0 || WeaponIndex >= WeaponData.Num())
    {
        UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid weapon index"));
        return;
    }

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (IsValid(Character))
    {
        if (IsValid(CurrentWeapon))
        {
            CurrentWeapon->StopFire();
            AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
        }

        CurrentWeapon = Weapons[WeaponIndex];

        const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data)
        {
            return Data.WeaponClass == CurrentWeapon->GetClass();
        });
        CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);

        EquipAnimInProgress = true;
        PlayAnimMontage(EquipAnimMontage);
    }
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (IsValid(Character))
    {
        Character->PlayAnimMontage(Animation);
    }
}

void USTUWeaponComponent::InitAnimations()
{
    auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);
    if (IsValid(EquipFinishedNotify))
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
    }
    else
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
        checkNoEntry();
    }

    for (auto OneWeaponData : WeaponData)
    {
        auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<USTUReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
        if (IsValid(ReloadFinishedNotify))
        {
            ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
        }
        else
        {
            UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify is forgotten to set"));
            checkNoEntry();
        }
    }
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (IsValid(Character) && Character->GetMesh() == MeshComponent)
    {
        EquipAnimInProgress = false;
    }
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (IsValid(Character) && Character->GetMesh() == MeshComponent)
    {
        ReloadAnimInProgress = false;
    }
}

bool USTUWeaponComponent::CanFire() const
{
    return IsValid(CurrentWeapon) && !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanEquip() const
{
    return !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanReload() const
{
    return IsValid(CurrentWeapon)
           && !EquipAnimInProgress
           && !ReloadAnimInProgress
           && CurrentWeapon->CanReload();
}

void USTUWeaponComponent::OnEmptyClip(ASTUBaseWeapon* AmmoEmptyWeapon)
{
    if (IsValid(AmmoEmptyWeapon))
    {
        if (CurrentWeapon == AmmoEmptyWeapon)
        {
            ChangeClip();
        }
        else
        {
            for (auto Weapon : Weapons)
            {
                if (Weapon == AmmoEmptyWeapon)
                {
                    Weapon->ChangeClip();
                }
            }
        }
    }
}

void USTUWeaponComponent::ChangeClip()
{
    if (CanReload())
    {
        CurrentWeapon->StopFire();
        CurrentWeapon->ChangeClip();

        ReloadAnimInProgress = true;
        PlayAnimMontage(CurrentReloadAnimMontage);
    }
}