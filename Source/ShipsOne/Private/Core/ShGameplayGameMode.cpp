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
	if (PlayersReadiness.Num() >= 2)
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_MoreThan2Detected,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	if (!IsValid(PC))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_ControllerNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	if (PlayersReadiness.Contains(PC))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_ControllerAlreadySet,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

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
	if (!IsValid(PC))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_ControllerNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	if (!PC->Implements<UGameplayNetworkInterface>())
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_ControllerNotImplementsInterface,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	
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

	if (!IsValid(GameManager))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_GameManagerInitFailure,
			TEXT(__FUNCTION__),
			*GetName());
		return false;
	}

	GameManager->InitializeSoloGameplay(PlayersReadiness[0].PCRef);
	
	return true;
}