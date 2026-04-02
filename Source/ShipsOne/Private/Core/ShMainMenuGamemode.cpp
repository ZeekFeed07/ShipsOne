#include "Core/ShMainMenuGamemode.h"
#include "Core/ShMainMenuPlayerController.h"
#include "Core/ShMainMenuPawn.h"
#include "Core/ShMainMenuHUD.h"

AShMainMenuGamemode::AShMainMenuGamemode()
{
	PlayerControllerClass = AShMainMenuPlayerController::StaticClass();
	DefaultPawnClass = AShMainMenuPawn::StaticClass();
	HUDClass = AShMainMenuHUD::StaticClass();
}
