// Shoot Them Up Game. All Rights Reserved.


#include "Menu/STUMenuWidget.h"

#include "STUGameInstance.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Menu/STULevelItemWidget.h"
#include "Sound/SoundCue.h"

void USTUMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (IsValid(StartGameButton))
    {
        StartGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame);
    }
    if (IsValid(QuitGameButton))
    {
        QuitGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnQuitGame);
    }

    InitLevelItems();
}

void USTUMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);
    FString Output = "";
    for (int32 i = 0; i < 4; ++i)
    {
        Output.Append(FString::ChrN(2, i % 2 ? '1' : '2'));
    }
    UE_LOG(LogTemp, Error, TEXT("%s"), *Output);

    if (Animation == HideAnimation)
    {
        const auto STUGameInstance = GetSTUGameInstance();

        if (!IsValid(STUGameInstance))
        {
            return;
        }

        UGameplayStatics::OpenLevel(this, STUGameInstance->GetStartupLevel().LevelName);
    }
}

void USTUMenuWidget::OnStartGame()
{
    if (IsValid(HideAnimation))
    {
        PlayAnimation(HideAnimation);
        UGameplayStatics::PlaySound2D(GetWorld(), StartGameSound);
    }
}

void USTUMenuWidget::OnQuitGame()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

void USTUMenuWidget::InitLevelItems()
{
    const auto STUGameInstance = GetSTUGameInstance();

    if (!IsValid(STUGameInstance))
    {
        return;
    }

    checkf(STUGameInstance->GetLevelData().Num()!=0, TEXT("Level data must not be empty!"));

    if (!IsValid(LevelItemsBox))
    {
        return;
    }
    LevelItemsBox->ClearChildren();

    for (auto LevelData : STUGameInstance->GetLevelData())
    {
        const auto LevelItemWidget = CreateWidget<USTULevelItemWidget>(GetWorld(), LevelItemWidgetClass);
        if (IsValid(LevelItemWidget))
        {
            LevelItemWidget->SetLevelData(LevelData);
            LevelItemWidget->OnLevelSelected.AddUObject(this, &USTUMenuWidget::OnLevelSelected);

            LevelItemsBox->AddChild(LevelItemWidget);
            LevelItemWidgets.Add(LevelItemWidget);
        }
    }

    if (STUGameInstance->GetStartupLevel().LevelName.IsNone())
    {
        OnLevelSelected(STUGameInstance->GetLevelData()[0]);
    }
    else
    {
        OnLevelSelected(STUGameInstance->GetStartupLevel());
    }
}

void USTUMenuWidget::OnLevelSelected(const FLevelData& LevelData)
{
    const auto STUGameInstance = GetSTUGameInstance();

    if (!IsValid(STUGameInstance))
    {
        return;
    }
    STUGameInstance->SetStartupLevel(LevelData);

    for (auto LevelItemWidget : LevelItemWidgets)
    {
        if (IsValid(LevelItemWidget))
        {
            const auto IsSelected = LevelData.LevelName == LevelItemWidget->GetLevelData().LevelName;
            LevelItemWidget->SetSelected(IsSelected);
        }
    }
}

USTUGameInstance* USTUMenuWidget::GetSTUGameInstance() const
{
    if (GetWorld())
    {
        return GetWorld()->GetGameInstance<USTUGameInstance>();
    }
    return nullptr;
}