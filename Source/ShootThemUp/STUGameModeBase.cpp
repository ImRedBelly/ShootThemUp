// Shoot Them Up Game. All Rights Reserved.


#include "STUGameModeBase.h"

#include "AIController.h"
#include "STURespawnComponent.h"
#include "EngineUtils.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "Player/STUPlayerState.h"
#include "UI/STUGameHUD.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseGameMode, All, All);

constexpr static int32 MinRoundTimeForRespawn = 10;

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
    PlayerStateClass = ASTUPlayerState::StaticClass();
}

void ASTUGameModeBase::StartPlay()
{
    Super::StartPlay();

    SpawnBots();
    CreateTeamsInfo();

    CurrentRound = 1;
    StartRound();

    SetMatchState(ESTUMatchState::InProgress);
}

UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (IsValid(InController) && InController->IsA<AAIController>())
    {
        return AIPawnClass;
    }

    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ASTUGameModeBase::Killed(AController* KillerController, AController* VictimController)
{
    const auto KillerPlayerState = IsValid(KillerController) ? Cast<ASTUPlayerState>(KillerController->PlayerState) : nullptr;
    const auto VictionPlayerState = IsValid(VictimController) ? Cast<ASTUPlayerState>(VictimController->PlayerState) : nullptr;

    if (IsValid(KillerPlayerState))
    {
        KillerPlayerState->AddKill();
    }
    if (IsValid(VictionPlayerState))
    {
        VictionPlayerState->AddDeath();
    }

    StartRespawn(VictimController);
}

void ASTUGameModeBase::RespawnRequest(AController* Controller)
{
    ResetOnePlayer(Controller);
}

bool ASTUGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
    if (PauseSet)
    {
        SetMatchState(Pause);
    }

    return PauseSet;
}

bool ASTUGameModeBase::ClearPause()
{
    const auto PauseCleared = Super::ClearPause();
    if (PauseCleared)
    {
        SetMatchState(InProgress);
    }

    return PauseCleared;
}

void ASTUGameModeBase::SpawnBots()
{
    if (!GetWorld()) return;
    for (int i = 0; i < GameData.PlayersNum - 1; ++i)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        const auto STUAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
        RestartPlayer(STUAIController);
    }
}

void ASTUGameModeBase::StartRound()
{
    RoundCountDown = GameData.RoundTime;
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUGameModeBase::GameTimerUpdate, 1.0f, true);
}

void ASTUGameModeBase::GameTimerUpdate()
{
    UE_LOG(LogBaseGameMode, Display, TEXT("UpdateTimer"));
    if (--RoundCountDown == 0)
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

        if (CurrentRound + 1 <= GameData.RoundsNum)
        {
            UE_LOG(LogBaseGameMode, Display, TEXT("===NewRound==="));
            ++CurrentRound;
            ResetPlayers();
            StartRound();
        }
        else
        {
            GameOver();
        }
    }
}

void ASTUGameModeBase::ResetPlayers()
{
    if (!GetWorld()) return;
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        ResetOnePlayer(It->Get());
    }
}

void ASTUGameModeBase::ResetOnePlayer(AController* Controller)
{
    if (IsValid(Controller) && IsValid(Controller->GetPawn()))
    {
        Controller->GetPawn()->Reset();
    }
    RestartPlayer(Controller);
    SetPlayerColor(Controller);
}

void ASTUGameModeBase::CreateTeamsInfo()
{
    if (!GetWorld()) return;

    int TeamID = 1;
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (IsValid(Controller))
        {
            const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
            if (IsValid(PlayerState))
            {
                PlayerState->SetTeamID(TeamID);
                PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
                PlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot");
                SetPlayerColor(Controller);

                TeamID = TeamID == 1 ? 2 : 1;
            }
        }
    }
}

FLinearColor ASTUGameModeBase::DetermineColorByTeamID(int32 TeamID) const
{
    if (TeamID - 1 < GameData.TeamsColor.Num())
    {
        return GameData.TeamsColor[TeamID - 1];
    }
    UE_LOG(LogBaseGameMode, Warning, TEXT("No color fir team id: %i"), TeamID);
    return GameData.DefaultTeamColor;
}

void ASTUGameModeBase::SetPlayerColor(AController* Controller)
{
    if (!IsValid(Controller)) return;

    const auto Character = Cast<ASTUBaseCharacter>(Controller->GetPawn());
    if (!IsValid(Character)) return;

    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
    if (!IsValid(PlayerState)) return;

    Character->SetPlayerColor(PlayerState->GetTeamColor());
}

void ASTUGameModeBase::LogPlayerInfo()
{
    if (!GetWorld()) return;
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (IsValid(Controller))
        {
            const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
            if (IsValid(PlayerState))
            {
                PlayerState->LogInfo();
            }
        }
    }
}

void ASTUGameModeBase::StartRespawn(AController* Controller)
{
    const auto RespawnAvailable = RoundCountDown > MinRoundTimeForRespawn + GameData.RespawnTime;

    if (RespawnAvailable)
    {
        const auto RespawnComponent = Controller->FindComponentByClass<USTURespawnComponent>();
        if (IsValid(RespawnComponent))
        {
            RespawnComponent->Respawn(GameData.RespawnTime);
        }
    }
}

void ASTUGameModeBase::GameOver()
{
    UE_LOG(LogBaseGameMode, Display, TEXT("======GameOver======"));
    LogPlayerInfo();

    for (auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        if (IsValid(Pawn))
        {
            Pawn->TurnOff();
            Pawn->DisableInput(nullptr);

        }
    }

    SetMatchState(ESTUMatchState::GameOver);
}

void ASTUGameModeBase::SetMatchState(ESTUMatchState State)
{
    if (MatchState != State)
    {
        MatchState = State;
        OnMatchStateChange.Broadcast(MatchState);
    }
}