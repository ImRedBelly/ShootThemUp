// Shoot Them Up Game. All Rights Reserved.


#include "UI/STUGameHUD.h"

#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"
#include "ShootThemUp/STUGameModeBase.h"
#include "UI/STUBaseWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameHUD, All, All);

void ASTUGameHUD::DrawHUD()
{
    Super::DrawHUD();
    //DrawCrossHair();
}

void ASTUGameHUD::BeginPlay()
{
    Super::BeginPlay();

    GameWidgets.Add(InProgress, CreateWidget<USTUBaseWidget>(GetWorld(), PlayerHUDWidgetClass));
    GameWidgets.Add(Pause, CreateWidget<USTUBaseWidget>(GetWorld(), PauseWidgetClass));
    GameWidgets.Add(GameOver, CreateWidget<USTUBaseWidget>(GetWorld(), GameOverWidgetClass));

    for (auto GameWidgetPair : GameWidgets)
    {
        const auto GameWidget = GameWidgetPair.Value;
        if (IsValid(GameWidget))
        {
            GameWidget->AddToViewport();
            GameWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    if (GetWorld())
    {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if (IsValid(GameMode))
        {
            GameMode->OnMatchStateChange.AddUObject(this, &ASTUGameHUD::OnMatchStateChange);
        }
    }
}

void ASTUGameHUD::DrawCrossHair()
{
    const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

    const float HalfLizeSize = 10.0f;
    const float LineThickness = 2.0f;
    const FColor LineColor = FColor::Green;

    DrawLine(Center.Min - HalfLizeSize, Center.Max, Center.Min + HalfLizeSize, Center.Max, LineColor, LineThickness);
    DrawLine(Center.Min, Center.Max - HalfLizeSize, Center.Min, Center.Max + HalfLizeSize, LineColor, LineThickness);
}

void ASTUGameHUD::OnMatchStateChange(ESTUMatchState State)
{
    if (IsValid(CurrentWidget))
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
        CurrentWidget = nullptr;
    }

    if (GameWidgets.Contains(State))
    {
        CurrentWidget = GameWidgets[State];
    }
    
    if (IsValid(CurrentWidget))
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
        CurrentWidget->Show();
    }

    UE_LOG(LogSTUGameHUD, Display, TEXT("Match state changed: %s"), *UEnum::GetValueAsString(State));
}