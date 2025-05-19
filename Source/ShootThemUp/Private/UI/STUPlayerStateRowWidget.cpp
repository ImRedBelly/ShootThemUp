// Shoot Them Up Game. All Rights Reserved.


#include "UI/STUPlayerStateRowWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void USTUPlayerStateRowWidget::SetPlayerName(const FText& Text)
{
    if (IsValid(PlayerNameTextBlock))
    {
        PlayerNameTextBlock->SetText(Text);
    }
}

void USTUPlayerStateRowWidget::SetKills(const FText& Text)
{
    if (IsValid(KillsTextBlock))
    {
        KillsTextBlock->SetText(Text);
    }
}

void USTUPlayerStateRowWidget::SetDeaths(const FText& Text)
{
    if (IsValid(DeathsTextBlock))
    {
        DeathsTextBlock->SetText(Text);
    }
}

void USTUPlayerStateRowWidget::SetTeam(const FText& Text)
{
    if (IsValid(TeamTextBlock))
    {
        TeamTextBlock->SetText(Text);
    }
}

void USTUPlayerStateRowWidget::SetPlayerIndicatorVisibility(bool Visible)
{
    if (IsValid(PlayerIndicatorImage))
    {
        PlayerIndicatorImage->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

void USTUPlayerStateRowWidget::SetTeamColor(const FLinearColor& Color)
{
    if (IsValid(TeamImage))
    {
        TeamImage->SetColorAndOpacity(Color);
    }
}