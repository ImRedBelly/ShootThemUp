// Shoot Them Up Game. All Rights Reserved.


#include "UI/STUGoToMenuWidget.h"

#include "STUGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void USTUGoToMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (IsValid(GoToMenuButton))
    {
        GoToMenuButton->OnClicked.AddDynamic(this, &USTUGoToMenuWidget::OnGoToMenu);
    }
}

void USTUGoToMenuWidget::OnGoToMenu()
{
    if (GetWorld())
    {
        const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
        if (IsValid(STUGameInstance) && !STUGameInstance->GetMenuLevelName().IsNone())
        {
            UGameplayStatics::OpenLevel(this, STUGameInstance->GetMenuLevelName());
        }
    }
}