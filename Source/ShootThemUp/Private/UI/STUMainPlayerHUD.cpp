// Shoot Them Up Game. All Rights Reserved.


#include "UI/STUMainPlayerHUD.h"

#include "STUHealthComponent.h"
#include "STUWeaponComponent.h"
#include "STUUtils.h"
#include "Components/ProgressBar.h"


float USTUMainPlayerHUD::GetHealthPercent() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());

    if (IsValid(HealthComponent))
    {
        return HealthComponent->GetHealthPercent();
    }

    return 0;
}

bool USTUMainPlayerHUD::TryGetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());

    if (IsValid(WeaponComponent) && WeaponComponent->TryGetCurrentWeaponUIData(UIData))
    {
        return true;
    }

    return false;
}

bool USTUMainPlayerHUD::TryGetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());

    if (IsValid(WeaponComponent) && WeaponComponent->TryGetCurrentWeaponAmmoData(AmmoData))
    {
        return true;
    }

    return false;
}

bool USTUMainPlayerHUD::IsPlayerAlive() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    return IsValid(HealthComponent) && !HealthComponent->IsDead();
}

bool USTUMainPlayerHUD::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    return IsValid(Controller) && Controller->GetStateName() == NAME_Spectating;
}

int32 USTUMainPlayerHUD::GetKillsNum() const
{
    const auto Controller = GetOwningPlayer();

    if (!IsValid(Controller)) return 0;

    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
    return IsValid(PlayerState) ? PlayerState->GetKillsNum() : 0;
}

FString USTUMainPlayerHUD::FormatBullets(int32 BulletsNum) const
{
    const int32 MaxLen = 3;
    const TCHAR PrefixSymbol = '0';

    auto BulletStr = FString::FromInt(BulletsNum);
    const auto SymbolNumToAdd = MaxLen - BulletStr.Len();

    if (SymbolNumToAdd > 0)
    {
        BulletStr = FString::ChrN(SymbolNumToAdd, PrefixSymbol).Append(BulletStr);
    }

    return BulletStr;
}

void USTUMainPlayerHUD::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (IsValid(GetOwningPlayer()))
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &USTUMainPlayerHUD::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
}

void USTUMainPlayerHUD::OnHealthChange(float Health, float HealthDelta)
{
    if (HealthDelta < 0)
    {
        OnTakeDamage();
        if (!IsAnimationPlaying(DamageAnimation))
        {
            PlayAnimation(DamageAnimation);
        }
    }
    UpdateHealthBar();
}

void USTUMainPlayerHUD::OnNewPawn(APawn* NewPawn)
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(NewPawn);

    if (IsValid(HealthComponent) && !HealthComponent->OnHealthChanged.IsBoundToObject(this))
    {
        HealthComponent->OnHealthChanged.AddUObject(this, &USTUMainPlayerHUD::OnHealthChange);
    }
    UpdateHealthBar();
}

void USTUMainPlayerHUD::UpdateHealthBar()
{
    if (IsValid(HealthProgressBar))
    {
        HealthProgressBar->SetFillColorAndOpacity(GetHealthPercent() > PercentColorThreshold ? GoodColor : BadColor);
    }
}