#include "Core/ShGameplayPlayerController.h"
#include "Core/ShGameplayGameMode.h"
#include "GameFramework/PlayerState.h"
#include "Data/GameplayData.h"
#include "Subsystems/ShGameManager.h"

AShGameplayPlayerController::AShGameplayPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AShGameplayPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitManager();
}

void AShGameplayPlayerController::InitManager()
{
	if (!GetWorld())
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_WorldNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	Manager = GetWorld()->GetSubsystem<UShGameManager>();
	if(!IsValid(Manager))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_SubsystemNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	APlayerState* State = GetPlayerState<APlayerState>();
	if (!IsValid(State))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_PlayerStateNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	Manager->SetupControllerRef(this);
	Manager->SetupPlayerStateRef(State);

	ServerControllerReadyRPC();
}

void AShGameplayPlayerController::ServerControllerReadyRPC_Implementation()
{
	if (!GetWorld())
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_WorldNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	AShGameplayGameMode* GM = GetWorld()->GetAuthGameMode<AShGameplayGameMode>();
	if (!IsValid(GM))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_GamemodeNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	GM->PlayerReady(this);
}
