#include "Core/ShGameplayGameMode.h"
#include "Core/ShGameplayPawn.h"
#include "Core/ShGameplayPlayerController.h"
#include "Core/ShMainPlayerState.h"
#include "Core/CoreManager.h"
#include "Interfaces/StateUtilityInterface.h"
#include "Interfaces/GameplayNetworkInterface.h"
#include "Data/GameplayData.h"

#include "Obejcts/AIBot.h"

DEFINE_LOG_CATEGORY(ShLog_Gameplay)

AShGameplayGameMode::AShGameplayGameMode()
{
	PlayerControllerClass = AShGameplayPlayerController::StaticClass();
	DefaultPawnClass = AShGameplayPawn::StaticClass();
	PlayerStateClass = AShMainPlayerState::StaticClass();
}

void AShGameplayGameMode::PlayerReady(APlayerController* PC)
{
	SH_VALIDATE(PlayersReadiness.Num() <= 2, LogMessage::MoreThan2Detected);
	SH_VALIDATE(IsValid(PC), LogMessage::ControllerNotValid);
	SH_VALIDATE(!PlayersReadiness.Contains(PC), LogMessage::ControllerAlreadySet);

	PlayersReadiness.Add({PC, true});

	if (GetNetMode() == NM_Standalone && PlayersReadiness.Num() == 1)
	{
		SendPrepareField(PC);

		InitializeGameManagerSolo();
		return;
	}

	if (PlayersReadiness.Num() == 2)
	{
		InitializeGameManagerMulti();
		// Comming soon...
	}

}

void AShGameplayGameMode::SendPrepareField(APlayerController* PC)
{
	SH_VALIDATE(IsValid(PC), LogMessage::ControllerNotValid);
	SH_VALIDATE(PC->Implements<UGameplayNetworkInterface>(), LogMessage::ControllerNotImplementsInterface);
	
	IGameplayNetworkInterface::Execute_ApproveFieldCreation(PC);
}

bool AShGameplayGameMode::InitializeGameManagerMulti()
{
	// Comming soon...
	return false;
}

bool AShGameplayGameMode::InitializeGameManagerSolo()
{
	GameManager = NewObject<UCoreManager>(this);

	SH_VALIDATE_RET(IsValid(GameManager), LogMessage::GameManagerInitFailure, false);

	GameManager->InitializeSoloGameplay(PlayersReadiness[0].PCRef);
	
	return true;
}