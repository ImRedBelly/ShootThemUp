// Shoot Them Up Game. All Rights Reserved.


#include "UI/STUGameHUD.h"

#include "Engine/Canvas.h"

void ASTUGameHUD::DrawHUD()
{
    Super::DrawHUD();
    DrawCrossHair();
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