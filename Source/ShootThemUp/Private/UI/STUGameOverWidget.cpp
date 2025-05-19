// Shoot Them Up Game. All Rights Reserved.


#include "UI/STUGameOverWidget.h"

#include "STUUtils.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Player/STUPlayerState.h"
#include "ShootThemUp/STUGameModeBase.h"
#include "UI/STUPlayerStateRowWidget.h"

void USTUGameOverWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetWorld())
    {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if (IsValid(GameMode))
        {
            GameMode->OnMatchStateChange.AddUObject(this, &USTUGameOverWidget::OnMatchStateChange);
        }
    }

    ResetLevelButton->OnClicked.AddDynamic(this, &USTUGameOverWidget::OnResetLevel);
}

void USTUGameOverWidget::OnMatchStateChange(ESTUMatchState State)
{
    if (State == GameOver)
    {
        UpdatePlayerStat();
    }
}

void USTUGameOverWidget::UpdatePlayerStat()
{
    if (GetWorld() && IsValid(PlayerStatBox))
    {
        PlayerStatBox->ClearChildren();

        for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
        {
            const auto Controller = It->Get();
            if (!IsValid(Controller)) continue;

            const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
            if (!IsValid(PlayerState)) continue;

            const auto PlayerStatRowWidget = CreateWidget<USTUPlayerStateRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
            if (!IsValid(PlayerStatRowWidget)) continue;

            PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
            PlayerStatRowWidget->SetKills(STUUtils::TextFromInt(PlayerState->GetKillsNum()));
            PlayerStatRowWidget->SetDeaths(STUUtils::TextFromInt(PlayerState->GetDeathsNum()));
            PlayerStatRowWidget->SetTeam(STUUtils::TextFromInt(PlayerState->GetTeamID()));
            PlayerStatRowWidget->SetPlayerIndicatorVisibility(Controller->IsPlayerController());
            PlayerStatRowWidget->SetTeamColor(PlayerState->GetTeamColor());

            PlayerStatBox->AddChild(PlayerStatRowWidget);
        }
    }
}

void USTUGameOverWidget::OnResetLevel()
{
    const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
    UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}