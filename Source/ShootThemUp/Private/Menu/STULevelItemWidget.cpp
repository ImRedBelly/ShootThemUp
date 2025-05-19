// Shoot Them Up Game. All Rights Reserved.


#include "Menu/STULevelItemWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USTULevelItemWidget::SetLevelData(const FLevelData& Data)
{
    LevelData = Data;

    if (IsValid(LevelNameTextBlock)) LevelNameTextBlock->SetText(FText::FromName(LevelData.LevelDisplayName));
    if (IsValid(LevelImage)) LevelImage->SetBrushFromTexture(LevelData.LevelThumb);
}

void USTULevelItemWidget::SetSelected(bool IsSelected)
{
    if (IsValid(LevelImage)) LevelImage->SetColorAndOpacity(IsSelected ? FLinearColor::Red : FLinearColor::White);
}

void USTULevelItemWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (IsValid(LevelSelectButton))
    {
        LevelSelectButton->OnClicked.AddDynamic(this, &USTULevelItemWidget::OnLevelItemClicked);
        LevelSelectButton->OnHovered.AddDynamic(this, &USTULevelItemWidget::OnLevelItemHovered);
        LevelSelectButton->OnUnhovered.AddDynamic(this, &USTULevelItemWidget::OnLevelItemUnhovered);
    }
}

void USTULevelItemWidget::OnLevelItemClicked()
{
    OnLevelSelected.Broadcast(LevelData);
}

void USTULevelItemWidget::OnLevelItemHovered()
{
    if (IsValid(FrameImage)) FrameImage->SetVisibility(ESlateVisibility::Visible);
}

void USTULevelItemWidget::OnLevelItemUnhovered()
{
    if (IsValid(FrameImage)) FrameImage->SetVisibility(ESlateVisibility::Hidden);
}