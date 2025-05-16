// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUBaseCharacter.h"
#include "STUPlayerCharacter.generated.h"

class USphereComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
struct FInputActionValue;
class UInputAction;

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerCharacter : public ASTUBaseCharacter
{
    GENERATED_BODY()

public:
    ASTUPlayerCharacter(const FObjectInitializer& ObjectInitializer);

    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    virtual bool IsRunning() const override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    USphereComponent* CameraCollisionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    UInputAction* SprintAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    UInputAction* FireAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    UInputAction* NextWeaponAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    UInputAction* ReloadAction;

    virtual void BeginPlay() override;
    virtual void OnDeath() override;

private:
    bool IsMovingForward = false;
    bool WantsToSprint = false;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

    void StartSprint();
    void StopSprint();


    UFUNCTION()
    void OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    void OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void CheckCameraOverlap();
};