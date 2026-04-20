#include "Helpers/HttpControllerBase.h"

UHttpControllerBase::UHttpControllerBase()
{
}

void UHttpControllerBase::RequestRoomsList()
{
	FHttpRequestRef ReceivingRoomsListRequest = FHttpModule::Get().CreateRequest();
	FString URL = HttpServer_Link + HttpServer_AllRoomsRoute;
	ReceivingRoomsListRequest->SetURL(URL); // http://localhost:8080/rooms
	ReceivingRoomsListRequest->SetVerb("GET");
	ReceivingRoomsListRequest->OnProcessRequestComplete().BindUObject(this, &UHttpControllerBase::OnRoomsListResponse);
	ReceivingRoomsListRequest->ProcessRequest();

	UE_LOG(ShLog_Connection, Display, TEXT("%s. FUNC: %s. Obj: %s."), *LogMessage_RequestRoomsList, TEXT(__FUNCTION__), *GetName());
}

void UHttpControllerBase::RequestCreateNewRoom(const FString& PlayerName)
{
	FHttpRequestRef CreatingRoomRequest = FHttpModule::Get().CreateRequest();
	CreatingRoomRequest->SetURL(HttpServer_Link + HttpServer_CreateRoom);
	CreatingRoomRequest->SetVerb("POST");
	CreatingRoomRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	FString RequestBody = FString::Printf(TEXT("{\"player_name\":\"%s\"}"), *PlayerName);

	CreatingRoomRequest->SetContentAsString(RequestBody);
	CreatingRoomRequest->OnProcessRequestComplete().BindUObject(this, &UHttpControllerBase::OnRoomCreationResponse);

	CreatingRoomRequest->ProcessRequest();

	UE_LOG(ShLog_Connection, Display, TEXT("%s FUNC: %s. Obj: %s"), *LogMessage_RequestCreateRoom, TEXT(__FUNCTION__), *GetName());
}

void UHttpControllerBase::RequestDeleteRoom()
{
	if (CurrentRoom.RoomID.IsEmpty())
	{
		UE_LOG(ShLog_Connection, Display, TEXT("%s FUNC: %s. Obj: %s"), *LogMessage_RoomIdEmpty, TEXT(__FUNCTION__), *GetName());
		return;
	}
	FHttpRequestRef DeleteRoomRequest = FHttpModule::Get().CreateRequest();
	DeleteRoomRequest->SetURL(HttpServer_Link + HttpServer_DeleteRoom + "/" + CurrentRoom.RoomID);
	DeleteRoomRequest->SetVerb("DELETE");
	DeleteRoomRequest->OnProcessRequestComplete().BindUObject(this, &UHttpControllerBase::OnRoomDeletionResponse);
	DeleteRoomRequest->ProcessRequest();

	UE_LOG(ShLog_Connection, Display, TEXT("%s FUNC: %s. Obj: %s"), *LogMessage_RequestDeleteRoom, TEXT(__FUNCTION__), *GetName());
}

void UHttpControllerBase::RequestJoinToRoom(const FString& PlayerName, const FString& RoomID)
{
	FHttpRequestRef JoinToRoomRequest = FHttpModule::Get().CreateRequest();

	JoinToRoomRequest->SetURL(HttpServer_Link + HttpServer_JoinRoom + RoomID + "/join");
	JoinToRoomRequest->SetVerb("POST");
	JoinToRoomRequest->OnProcessRequestComplete().BindUObject(this, &UHttpControllerBase::OnRoomJoiningResponse);
	JoinToRoomRequest->ProcessRequest();

	UE_LOG(ShLog_Connection, Display, TEXT("%s FUNC: %s. Obj: %s"), *LogMessage_RequestJoinRoom, TEXT(__FUNCTION__), *GetName());
}

void UHttpControllerBase::OnRoomsListResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
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

void UHttpControllerBase::OnRoomCreationResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	CurrentRoom = FRoomInfo();

	if (!bSuccess || !Response.IsValid())
	{
		UE_LOG(ShLog_Connection, Error, TEXT("%s Func: %s. Obj: %s."), *LogMessage_ReachingServerFail, TEXT(__FUNCTION__), *GetName());
		OnRoomCreated.Broadcast(false, 0, CurrentRoom);
		return;
	}

	int32 Code = Response->GetResponseCode();
	if (Code < 200 || Code >= 400)
	{
		// TODO: обработать каждый случай с кодом 400, 404
		OnRoomCreated.Broadcast(false, Code, CurrentRoom);
		return;
	}

	FString Body = Response->GetContentAsString();
	TSharedPtr<FJsonValue> JsonValue;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Body);
	if (!FJsonSerializer::Deserialize(JsonReader, JsonValue))
	{
		UE_LOG(ShLog_Connection, Error, TEXT("%s Func: %s. Obj: %s."), *LogMessage_DeserializeRoomCreationFailed, TEXT(__FUNCTION__), *GetName());
		OnRoomCreated.Broadcast(false, Code, CurrentRoom);
		return;
	}

	TSharedPtr<FJsonObject> JsonObject = JsonValue->AsObject();
	if (!JsonObject.IsValid())
	{
		UE_LOG(ShLog_Connection, Error, TEXT("%s Func: %s. Obj: %s."), *LogMessage_GettingCreatedRoomAsObjectFailed, TEXT(__FUNCTION__), *GetName());
		OnRoomCreated.Broadcast(false, Code, CurrentRoom);
		return;
	}

	if (!JsonObject->TryGetStringField(JsonData_RoomId, CurrentRoom.RoomID) ||
		!JsonObject->TryGetStringField(JsonData_RoomHost, CurrentRoom.HostName) ||
		!JsonObject->TryGetStringField(JsonData_RoomCreationTime, CurrentRoom.CreatedTime))
	{
		UE_LOG(ShLog_Connection, Error, TEXT("%s Func: %s. Obj: %s."), *LogMessage_GetFieldsRoomCreationFailed, TEXT(__FUNCTION__), *GetName());
		OnRoomCreated.Broadcast(false, Code, CurrentRoom);
		return;
	}

	OnRoomCreated.Broadcast(true, Code, CurrentRoom);
}

void UHttpControllerBase::OnRoomDeletionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
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

void UHttpControllerBase::OnRoomJoiningResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	CurrentRoom = {};

	if (!bSuccess || !Response.IsValid())
	{
		OnJoinedToRoom.Broadcast(false, 0, CurrentRoom);
		return;
	}

	int32 Code = Response->GetResponseCode();

	if (Code < 200 || Code >= 400)
	{
		// TODO: обработать каждый случай с кодом 400, 404
		OnJoinedToRoom.Broadcast(false, Code, CurrentRoom);
		return;
	}

	FString Body = Response->GetContentAsString();
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Body);
	TSharedPtr<FJsonValue> JsonValue;
	if (!FJsonSerializer::Deserialize(JsonReader, JsonValue))
	{
		return;
	}

	TSharedPtr<FJsonObject> JsonObject = JsonValue->AsObject();

	if (!JsonObject.IsValid())
	{
		return;
	}

	JsonObject->TryGetStringField(JsonData_RoomId, CurrentRoom.RoomID);
	JsonObject->TryGetStringField(JsonData_RoomHost, CurrentRoom.HostName);
	JsonObject->TryGetBoolField(JsonData_RoomHostReadiness, CurrentRoom.HostReadiness);
	JsonObject->TryGetStringField(JsonData_RoomGuest, CurrentRoom.GuestName);
	JsonObject->TryGetBoolField(JsonData_RoomGuestReadiness, CurrentRoom.GuestReadiness);
	JsonObject->TryGetStringField(JsonData_RoomStatus, CurrentRoom.RoomStatus);
	JsonObject->TryGetStringField(JsonData_RoomCreationTime, CurrentRoom.CreatedTime);

	OnJoinedToRoom.Broadcast(true, Code, CurrentRoom);
}

FRoomInfo UHttpControllerBase::GetCurrentRoom() const
{
	return CurrentRoom;
}
