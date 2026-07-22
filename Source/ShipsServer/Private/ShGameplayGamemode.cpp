#include "ShGameplayGamemode.h"

#include "ShGameplayGamestate.h"
#include "ShGameplayPlayerState.h"
#include "ShGameplayPlayerController.h"
#include "ShGameplayPawn.h"

AShGameplayGamemode::AShGameplayGamemode()
{
	GameStateClass			= AShGameplayGamestate::StaticClass();
	PlayerStateClass		= AShGameplayPlayerState::StaticClass();
	PlayerControllerClass	= AShGameplayPlayerController::StaticClass();
	DefaultPawnClass		= AShGameplayPawn::StaticClass();
}