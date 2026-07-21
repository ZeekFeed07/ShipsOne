#include "Core/ShMainMenuPlayerController.h"
#include "Data/UIData.h"
#include "Interfaces/HudUtility.h"
#include "GameFramework/HUD.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Helpers/HttpControllerBase.h"

DEFINE_LOG_CATEGORY(ShLog_Connection);

AShMainMenuPlayerController::AShMainMenuPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AShMainMenuPlayerController::RequestRoomsList_Implementation()
{
	HttpController->RequestRoomsList();
}

void AShMainMenuPlayerController::RequestCreateNewRoom_Implementation(const FString& PlayerName)
{
	HttpController->RequestCreateNewRoom(PlayerName);
}

FRoomInfo AShMainMenuPlayerController::GetCurrentRoom_Implementation() const
{
	return HttpController->GetCurrentRoom();
}

void AShMainMenuPlayerController::RequestDeleteRoom_Implementation()
{
	HttpController->RequestDeleteRoom();
}

void AShMainMenuPlayerController::RequestJoinToRoom_Implementation(const FString& PlayerName, const FString& RoomID)
{
	HttpController->RequestJoinToRoom(PlayerName, RoomID);
}

void AShMainMenuPlayerController::BindToRoomsListReceived_Implementation(const FOnHttpServerRoomsListReceived& Event)
{
	HttpController->OnRoomsListReceived.Add(Event);
}

void AShMainMenuPlayerController::UnbindFromRoomsListReceived_Implementation(const FOnHttpServerRoomsListReceived& Event)
{
	HttpController->OnRoomsListReceived.Remove(Event);
}

void AShMainMenuPlayerController::BindToRoomCreated_Implementation(const FOnHttpServerRoomCreation& Event)
{
	HttpController->OnRoomCreated.Add(Event);
}

void AShMainMenuPlayerController::UnbindFromRoomCreated_Implementation(const FOnHttpServerRoomCreation& Event)
{
	HttpController->OnRoomCreated.Remove(Event);
}

void AShMainMenuPlayerController::BindToRoomDeleted_Implementation(const FOnHttpServerRoomDeletion& Event)
{
	HttpController->OnRoomDeleted.Add(Event);
}

void AShMainMenuPlayerController::UnbindFromRoomDeleted_Implementation(const FOnHttpServerRoomDeletion& Event)
{
	HttpController->OnRoomDeleted.Remove(Event);
}

void AShMainMenuPlayerController::BindToRoomJoining_Implementation(const FOnHttpServerJoiningRoom& Event)
{
	HttpController->OnJoinedToRoom.Add(Event);
}

void AShMainMenuPlayerController::UnbindFromRoomJoining_Implementation(const FOnHttpServerJoiningRoom& Event)
{
	HttpController->OnJoinedToRoom.Remove(Event);
}

void AShMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(GetHUD()))
	{
		UE_LOG(ShLog_UI, Error, TEXT("%s Func: %s. Obj: %s."), *LogMessage_HudObjectInvalid, TEXT(__FUNCTION__), *GetName());
		return;
	}

	if (!GetHUD()->Implements<UHudUtility>())
	{
		UE_LOG(ShLog_UI, Error, TEXT("%s Func: %s. Obj: %s."), *LogMessage_HudInterfaceInvalid, TEXT(__FUNCTION__), *GetName());
		return;
	}

	IHudUtility::Execute_InitMainWidget(GetHUD());

	HttpController = NewObject<UHttpControllerBase>(this);

	SetupInputMode();
}

void AShMainMenuPlayerController::SetupInputMode()
{
	FInputModeGameAndUI InputMode = {};
	InputMode.SetHideCursorDuringCapture(false);

	SetInputMode(InputMode);

	bShowMouseCursor = true;
}