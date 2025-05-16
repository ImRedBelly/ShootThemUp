// Shoot Them Up Game. All Rights Reserved.


#include "Player/STUPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "STUWeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

ASTUPlayerCharacter::ASTUPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0, 100, 80);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
    CameraCollisionComponent->SetupAttachment(CameraComponent);
    CameraCollisionComponent->SetSphereRadius(10.0f);
    CameraCollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void ASTUPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    check(PlayerInputComponent)
    check(WeaponComponent)

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->Player);

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
    {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::Move);

        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::Look);

        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASTUPlayerCharacter::OnStartJump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASTUPlayerCharacter::OnStopJump);

        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ASTUPlayerCharacter::StartSprint);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASTUPlayerCharacter::StopSprint);

        EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, WeaponComponent, &USTUWeaponComponent::StartFire);
        EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, WeaponComponent, &USTUWeaponComponent::StopFire);

        EnhancedInputComponent->BindAction(NextWeaponAction, ETriggerEvent::Triggered, WeaponComponent, &USTUWeaponComponent::NextWeapon);
        EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, WeaponComponent, &USTUWeaponComponent::Reload);
    }
}

bool ASTUPlayerCharacter::IsRunning() const
{
    return WantsToSprint && IsMovingForward && !GetVelocity().IsZero();
}

void ASTUPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(CameraCollisionComponent);

    CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionBeginOverlap);
    CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionEndOverlap);
}

void ASTUPlayerCharacter::OnDeath()
{
    Super::OnDeath();

    if (GetController())
    {
        GetController()->ChangeState(NAME_Spectating);
    }
}

void ASTUPlayerCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    IsMovingForward = MovementVector.Y > 0 && MovementVector.X == 0;

    if (Controller != nullptr)
    {
        AddMovementInput(GetActorForwardVector(), MovementVector.Y);
        AddMovementInput(GetActorRightVector(), MovementVector.X);
    }
}

void ASTUPlayerCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}


void ASTUPlayerCharacter::StartSprint()
{
    WantsToSprint = true;
}

void ASTUPlayerCharacter::StopSprint()
{
    WantsToSprint = false;
}

void ASTUPlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    CheckCameraOverlap();
}

void ASTUPlayerCharacter::OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    CheckCameraOverlap();
}

void ASTUPlayerCharacter::CheckCameraOverlap()
{
    const auto HideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
    GetMesh()->SetOwnerNoSee(HideMesh);

    TArray<USceneComponent*> MeshChildren;
    GetMesh()->GetChildrenComponents(true, MeshChildren);

    for (auto MeshChild : MeshChildren)
    {
        const auto MeshChildGeometry = Cast<UPrimitiveComponent>(MeshChild);
        if (IsValid(MeshChildGeometry))
        {
            MeshChildGeometry->SetOwnerNoSee(HideMesh);
        }
    }
}