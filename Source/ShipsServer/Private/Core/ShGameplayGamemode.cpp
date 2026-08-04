#include "Core/ShGameplayGamemode.h"

#include "Core/ShGameplayGamestate.h"
#include "Core/ShGameplayPlayerState.h"
#include "Core/ShGameplayPlayerController.h"
#include "Core/ShGameplayPawn.h"

#include "Data/GameplayData.h"

DEFINE_LOG_CATEGORY(ShLog_Gameplay);

AShGameplayGamemode::AShGameplayGamemode()
{
	GameStateClass			= AShGameplayGamestate::StaticClass();
	PlayerStateClass		= AShGameplayPlayerState::StaticClass();
	PlayerControllerClass	= AShGameplayPlayerController::StaticClass();
	DefaultPawnClass		= AShGameplayPawn::StaticClass();
}

void AShGameplayGamemode::PreLogin(const FString& Options, const FString& Address,
	const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{

}

void AShGameplayGamemode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	LoggedPlayers.Add(NewPlayer);

	AShGameplayGamestate* GameplayGameState = GetGameState<AShGameplayGamestate>();

	SH_VALIDATE(GameplayGameState, LogMessage::GameStateNotValid);

	if (IsAllPlayersLogged())
	{
		GameplayGameState->bBothLogged = true;
	}
}

bool AShGameplayGamemode::IsAllPlayersLogged() const
{
	return LoggedPlayers.Num() == MaxPlayers;
}
