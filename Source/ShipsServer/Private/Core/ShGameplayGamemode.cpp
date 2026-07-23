#include "Core/ShGameplayGamemode.h"

#include "Core/ShGameplayGamestate.h"
#include "Core/ShGameplayPlayerState.h"
#include "Core/ShGameplayPlayerController.h"
#include "Core/ShGameplayPawn.h"

AShGameplayGamemode::AShGameplayGamemode()
{
	GameStateClass			= AShGameplayGamestate::StaticClass();
	PlayerStateClass		= AShGameplayPlayerState::StaticClass();
	PlayerControllerClass	= AShGameplayPlayerController::StaticClass();
	DefaultPawnClass		= AShGameplayPawn::StaticClass();
}