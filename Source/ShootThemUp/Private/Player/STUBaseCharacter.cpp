// Shoot Them Up Game. All Rights Reserved.

#include "Player/STUBaseCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "STUWeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(CharacterMovementComponentName))
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0, 100, 80);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");
    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
    HealthTextComponent->SetOwnerNoSee(true);
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent)
    check(HealthTextComponent)
    check(GetCharacterMovement())
    check(GetMesh())

    OnHealthChanged(HealthComponent->GetHealth(), 0);

    HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::Move);

        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::Look);

        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASTUBaseCharacter::OnStartJump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASTUBaseCharacter::OnStopJump);

        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ASTUBaseCharacter::StartSprint);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASTUBaseCharacter::StopSprint);

        EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, WeaponComponent, &USTUWeaponComponent::StartFire);
        EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, WeaponComponent, &USTUWeaponComponent::StopFire);

        EnhancedInputComponent->BindAction(NextWeaponAction, ETriggerEvent::Triggered, WeaponComponent, &USTUWeaponComponent::NextWeapon);
        EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, WeaponComponent, &USTUWeaponComponent::Reload);
    }
}

void ASTUBaseCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    IsMovingForward = MovementVector.Y > 0 && MovementVector.X == 0;

    if (Controller != nullptr)
    {
        AddMovementInput(GetActorForwardVector(), MovementVector.Y);
        AddMovementInput(GetActorRightVector(), MovementVector.X);
    }
}

void ASTUBaseCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}


void ASTUBaseCharacter::OnStartJump()
{
    Jump();
}

void ASTUBaseCharacter::OnStopJump()
{
    StopJumping();
}

void ASTUBaseCharacter::StartSprint()
{
    WantsToSprint = true;
}

void ASTUBaseCharacter::StopSprint()
{
    WantsToSprint = false;
}

bool ASTUBaseCharacter::IsRunning() const
{
    return WantsToSprint && IsMovingForward && !GetVelocity().IsZero();
}

float ASTUBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero())
    {
        return 0.0f;
    }
    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngelBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);

    const auto Degrees = FMath::RadiansToDegrees(AngelBetween);

    return CrossProduct.IsZero() ? Degrees : FMath::RadiansToDegrees(AngelBetween) * FMath::Sign(CrossProduct.Z);
}

void ASTUBaseCharacter::OnDeath()
{
    //PlayAnimMontage(DeathAnimMontage);

    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(LifeSpanOnDeath);

    if (GetController())
    {
        GetController()->ChangeState(NAME_Spectating);
    }

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);   
    WeaponComponent->StopFire();

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
    
    
    UE_LOG(LogBaseCharacter, Display, TEXT("Player %s id dead"), *GetName());
}

void ASTUBaseCharacter::OnHealthChanged(float Health, float HealthDelta)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    const auto FallVelocityZ = -GetVelocity().Z;
    UE_LOG(LogBaseCharacter, Display, TEXT("On landed: %f"), FallVelocityZ);

    if (FallVelocityZ < LandedDamageVelocity.X)
    {
        return;
    }

    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);

    UE_LOG(LogBaseCharacter, Display, TEXT("FinalDamage: %f"), FinalDamage);
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}