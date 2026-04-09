#include "Core/ShMainMenuPlayerController.h"
#include "Data/UIData.h"
#include "Interfaces/HudUtility.h"
#include "GameFramework/HUD.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY(ShLog_Connection);

AShMainMenuPlayerController::AShMainMenuPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AShMainMenuPlayerController::RequestQuitGame_Implementation()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, false);
}

void AShMainMenuPlayerController::RequestRoomsList_Implementation()
{
	FHttpRequestRef ReceivingRoomsListRequest = FHttpModule::Get().CreateRequest();
	FString URL = HttpServer_Link + HttpServer_AllRoomsRoute;
	ReceivingRoomsListRequest->SetURL(URL); // http://localhost:8080/rooms
	ReceivingRoomsListRequest->SetVerb("GET");
	ReceivingRoomsListRequest->OnProcessRequestComplete().BindUObject(this, &AShMainMenuPlayerController::OnHttpServerRoomsListReceived);
	ReceivingRoomsListRequest->ProcessRequest();

	UE_LOG(ShLog_Connection, Display, TEXT("%s. FUNC: %s. Obj: %s."), *LogMessage_RequestRoomsList, TEXT(__FUNCTION__), *GetName());
}

void AShMainMenuPlayerController::RequestCreateNewRoom_Implementation(const FString& PlayerName)
{
	FHttpRequestRef CreatingRoomRequest = FHttpModule::Get().CreateRequest();
	CreatingRoomRequest->SetURL(HttpServer_Link + HttpServer_CreateRoom);
	CreatingRoomRequest->SetVerb("POST");
	CreatingRoomRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	FString RequestBody = FString::Printf(TEXT("{\"player_name\":\"%s\"}"), *PlayerName);

	CreatingRoomRequest->SetContentAsString(RequestBody);
	CreatingRoomRequest->OnProcessRequestComplete().BindUObject(this, &AShMainMenuPlayerController::OnHttpServerRoomCreated);
	
	CreatingRoomRequest->ProcessRequest();

	UE_LOG(ShLog_Connection, Display, TEXT("%s FUNC: %s. Obj: %s"), *LogMessage_RequestCreateRoom, TEXT(__FUNCTION__), *GetName());
}

FRoomInfo AShMainMenuPlayerController::GetCreatedRoom_Implementation() const
{
	return CreatedRoom;
}

void AShMainMenuPlayerController::RequestDeleteRoom_Implementation()
{
	if (CreatedRoom.RoomID.IsEmpty())
	{
		UE_LOG(ShLog_Connection, Display, TEXT("%s FUNC: %s. Obj: %s"), *LogMessage_RoomIdEmpty, TEXT(__FUNCTION__), *GetName());
		return;
	}
	FHttpRequestRef DeleteRoomRequest = FHttpModule::Get().CreateRequest();
	DeleteRoomRequest->SetURL(HttpServer_Link + HttpServer_DeleteRoom + "/" + CreatedRoom.RoomID);
	DeleteRoomRequest->SetVerb("DELETE");
	DeleteRoomRequest->OnProcessRequestComplete().BindUObject(this, &AShMainMenuPlayerController::OnHttpServerRoomDeleted);
	DeleteRoomRequest->ProcessRequest();

	UE_LOG(ShLog_Connection, Display, TEXT("%s FUNC: %s. Obj: %s"), *LogMessage_RequestDeleteRoom, TEXT(__FUNCTION__), *GetName());
}

void AShMainMenuPlayerController::BindToRoomsListReceived_Implementation(const FOnHttpServerRoomsListReceived& Event)
{
	OnRoomsListReceived.Add(Event);
}

void AShMainMenuPlayerController::UnbindFromRoomsListReceived_Implementation(const FOnHttpServerRoomsListReceived& Event)
{
	OnRoomsListReceived.Remove(Event);
}

void AShMainMenuPlayerController::BindToRoomCreated_Implementation(const FOnHttpServerRoomCreation& Event)
{
	OnRoomCreated.Add(Event);
}

void AShMainMenuPlayerController::UnbindFromRoomCreated_Implementation(const FOnHttpServerRoomCreation& Event)
{
	OnRoomCreated.Remove(Event);
}

void AShMainMenuPlayerController::BindToRoomDeleted_Implementation(const FOnHttpServerRoomDeletion& Event)
{
	OnRoomDeleted.Add(Event);
}

void AShMainMenuPlayerController::UnbindFromRoomDeleted_Implementation(const FOnHttpServerRoomDeletion& Event)
{
	OnRoomDeleted.Remove(Event);
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

	SetupInputMode();
}

void AShMainMenuPlayerController::SetupInputMode()
{
	FInputModeGameAndUI InputMode = FInputModeGameAndUI();
	InputMode.SetHideCursorDuringCapture(false);

	SetInputMode(InputMode);

	bShowMouseCursor = true;
}

void AShMainMenuPlayerController::OnHttpServerRoomsListReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	TArray<FRoomInfo> RoomList = TArray<FRoomInfo>();

	if (!bSuccess || !Response.IsValid())
	{
		UE_LOG(ShLog_Connection, Error, TEXT("%s Func: %s. Obj: %s."), *LogMessage_ReachingServerFail, TEXT(__FUNCTION__), *GetName());
		OnRoomsListReceived.Broadcast(false, 0, RoomList);
		return;
	}

	int32 Code = Response->GetResponseCode();
	if (Code < 200 || Code >= 400)
	{
		OnRoomsListReceived.Broadcast(false, Code, RoomList);
		return;
	}

	FString Body = Response->GetContentAsString();
	TArray<TSharedPtr<FJsonValue>> JsonArray;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Body);

	if (!FJsonSerializer::Deserialize(JsonReader, JsonArray))
	{
		UE_LOG(ShLog_Connection, Error, TEXT("%s FUNC: %s. Obj: %s."), *LogMessage_DeserializeRoomsListFailed, TEXT(__FUNCTION__), *GetName());
		OnRoomsListReceived.Broadcast(false, Code, RoomList);
		return;
	}

	for (const auto& JsonValue : JsonArray)
	{
		TSharedPtr<FJsonObject> Obj = JsonValue->AsObject();

		if (!Obj.IsValid()) continue;

		FRoomInfo Room;

		if (!Obj->TryGetStringField(JsonData_RoomId, Room.RoomID)) continue;
		if (!Obj->TryGetStringField(JsonData_RoomHost, Room.HostName)) continue;
		if (!Obj->TryGetStringField(JsonData_RoomCreationTime, Room.CreatedTime)) continue;

		RoomList.Add(Room);
	}

	OnRoomsListReceived.Broadcast(bSuccess, Code, RoomList);
	UE_LOG(ShLog_Connection, Display, TEXT("%s FUNC: %s. Obj: %s"), *LogMessage_ReceivedRoomsListSuccessfully, TEXT(__FUNCTION__), *GetName());
}

	void AShMainMenuPlayerController::OnHttpServerRoomCreated(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
	{
		CreatedRoom = FRoomInfo();

		if (!bSuccess || !Response.IsValid())
		{
			UE_LOG(ShLog_Connection, Error, TEXT("%s Func: %s. Obj: %s."), *LogMessage_ReachingServerFail, TEXT(__FUNCTION__), *GetName());
			OnRoomCreated.Broadcast(false, 0, CreatedRoom);
			return;
		}

		int32 Code = Response->GetResponseCode();
		if (Code < 200 || Code >= 400)
		{
			// TODO: обработать каждый случай с кодом 400, 404
			OnRoomCreated.Broadcast(false, Code, CreatedRoom);
			return;
		}

		FString Body = Response->GetContentAsString();
		TSharedPtr<FJsonValue> JsonValue;
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Body);
		if (!FJsonSerializer::Deserialize(JsonReader, JsonValue))
		{
			UE_LOG(ShLog_Connection, Error, TEXT("%s Func: %s. Obj: %s."), *LogMessage_DeserializeRoomCreationFailed, TEXT(__FUNCTION__), *GetName());
			OnRoomCreated.Broadcast(false, Code, CreatedRoom);
			return;
		}

		TSharedPtr<FJsonObject> JsonObject = JsonValue->AsObject();
		if (!JsonObject.IsValid())
		{
			UE_LOG(ShLog_Connection, Error, TEXT("%s Func: %s. Obj: %s."), *LogMessage_GettingCreatedRoomAsObjectFailed, TEXT(__FUNCTION__), *GetName());
			OnRoomCreated.Broadcast(false, Code, CreatedRoom);
			return;
		}

		if (!JsonObject->TryGetStringField(JsonData_RoomId, CreatedRoom.RoomID) ||
			!JsonObject->TryGetStringField(JsonData_RoomHost, CreatedRoom.HostName) ||
			!JsonObject->TryGetStringField(JsonData_RoomCreationTime, CreatedRoom.CreatedTime))
		{
			UE_LOG(ShLog_Connection, Error, TEXT("%s Func: %s. Obj: %s."), *LogMessage_GetFieldsRoomCreationFailed, TEXT(__FUNCTION__), *GetName());
			OnRoomCreated.Broadcast(false, Code, CreatedRoom);
			return;
		}

		OnRoomCreated.Broadcast(true, Code, CreatedRoom);
	}

	void AShMainMenuPlayerController::OnHttpServerRoomDeleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
	{
		FString DeletedRoomID = FString();
		if (!bSuccess || !Response.IsValid())
		{
			UE_LOG(ShLog_Connection, Error, TEXT("%s Func: %s. Obj: %s."), *LogMessage_ReachingServerFail, TEXT(__FUNCTION__), *GetName());
			OnRoomDeleted.Broadcast(false, 0, DeletedRoomID);
			return;
		}

		int32 Code = Response->GetResponseCode();
		if (Code < 200 || Code >= 400)
		{
			// TODO: обработать каждый случай с кодом 400, 404
			OnRoomDeleted.Broadcast(false, Code, DeletedRoomID);
			return;
		}

		FString Body = Response->GetContentAsString();
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Body);
		TSharedPtr<FJsonValue> JsonValue;
		if (!FJsonSerializer::Deserialize(JsonReader, JsonValue))
		{
			UE_LOG(ShLog_Connection, Error, TEXT("%s Func: %s. Obj: %s."), *LogMessage_DeserializeRoomDeletionFailed, TEXT(__FUNCTION__), *GetName());
			OnRoomDeleted.Broadcast(false, Code, DeletedRoomID);
			return;
		}
		TSharedPtr<FJsonObject> JsonObject = JsonValue->AsObject();

		if (!JsonObject.IsValid())
		{
			UE_LOG(ShLog_Connection, Error, TEXT("%s Func: %s. Obj: %s."), *LogMessage_GettingDeletedRoomAsObjectFailed, TEXT(__FUNCTION__), *GetName());
			OnRoomDeleted.Broadcast(false, Code, DeletedRoomID);
			return;
		}

		JsonObject->TryGetStringField(JsonData_RoomId, DeletedRoomID);


		OnRoomDeleted.Broadcast(true, Code, DeletedRoomID);
	}
