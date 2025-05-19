// Shoot Them Up Game. All Rights Reserved.


#include "Menu/STUMenuHUD.h"

#include "Blueprint/UserWidget.h"
#include "Menu/STUMenuWidget.h"

void ASTUMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    if (IsValid(MenuWidgetClass))
    {
        const auto MenuWidget = CreateWidget<USTUBaseWidget>(GetWorld(), MenuWidgetClass);
        if (IsValid(MenuWidget))
        {
            MenuWidget->AddToViewport();
            MenuWidget->Show();
        }
    }
}