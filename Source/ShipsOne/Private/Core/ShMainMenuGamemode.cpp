#include "Core/ShMainMenuGamemode.h"
#include "Core/ShMainMenuPlayerController.h"
#include "Core/ShMainMenuPawn.h"

AShMainMenuGamemode::AShMainMenuGamemode()
{
	PlayerControllerClass = AShMainMenuPlayerController::StaticClass();
	DefaultPawnClass = AShMainMenuPawn::StaticClass();
}
