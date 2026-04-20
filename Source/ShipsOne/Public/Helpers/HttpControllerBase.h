#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/ConnectionData.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpControllerBase.generated.h"

UCLASS()
class SHIPSONE_API UHttpControllerBase : public UObject
{
	GENERATED_BODY()
	
public:
	UHttpControllerBase();
public:
	/**
	* @brief Отправка запроса на http сервер на получние списка комнат
	*/
	virtual void RequestRoomsList();
	/**
	* @brief Отправка запроса на http сервер на создание новой комнаты
	*
	* @param PlayerName - имя игрока
	*/
	virtual void RequestCreateNewRoom(const FString& PlayerName);
	/**
	* @brief Удалить текущую комнату созданную комнату
	*/
	virtual void RequestDeleteRoom();

	virtual void RequestJoinToRoom(const FString& PlayerName, const FString& RoomID);
	/**
	* @brief Получить созданную комнату
	*
	* @return - информация о созданной комнате
	*/
	virtual FRoomInfo GetCurrentRoom() const;

protected:
	/**
	* @brief Получение ответа от http сервера на запрос о получении списка комнат
	*
	* @param Request - отправленный запрос
	* @param Response - полученный ответ
	* @param bSuccess - успешость запроса
	*/
	virtual void OnRoomsListResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);
	/**
	* @brief Получение ответа от http сервера на запрос о создании новой комнаты
	*
	* @param Request - отправленный запрос
	* @param Response - полученный ответ
	* @param bSuccess - успешость запроса
	*/
	virtual void OnRoomCreationResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);
	/**
	* @brief Получение ответа от http сервера на запрос об удалении текущей комнаты
	*
	* @param Request - отправленный запрос
	* @param Response - полученный ответ
	* @param bSuccess - успешость запроса
	*/
	virtual void OnRoomDeletionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

	virtual void OnRoomJoiningResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);
public:
	/**
	* @brief Делегат для оповещения о получении списка комнат
	*
	* @param bSuccess - успешность запроса
	* @param Code - код от сервера
	* @param RoomsList - полученный список комнат
	*/
	UPROPERTY(BlueprintAssignable, Category = "Server")
	FOnHttpServerRoomsListReceivedMulticast OnRoomsListReceived;
	/**
	* @brief Делегат для оповещения о получении новой созданной комнаты
	*
	* @param bSuccess - успешность запроса
	* @param Code - код от сервера
	* @param NewRoom - информация о новой комнате
	*/
	UPROPERTY(BlueprintAssignable, Category = "Server")
	FOnHttpServerRoomCreationMulticast OnRoomCreated;
	/**
	* @brief Делегат для оповещения об удалении текущей комнаты
	*
	* @param bSuccess - успешность запроса
	* @param Code - код от сервера
	* @param RoomID - ID удалённой комнаты
	*/
	UPROPERTY(BlueprintAssignable, Category = "Server")
	FOnHttpServerRoomDeletionMulticast OnRoomDeleted;

	UPROPERTY(BlueprintAssignable, Category = "Server")
	FOnHttpServerJoiningRoomMulticast OnJoinedToRoom;

protected:
	FString HttpServer_Link = "http://127.0.0.1:8080";

	FRoomInfo CurrentRoom;

	// ============================== REST API ==============================//
	FString HttpServer_AllRoomsRoute						= TEXT("/rooms");
	FString HttpServer_CreateRoom							= TEXT("/rooms");
	FString HttpServer_DeleteRoom							= TEXT("/rooms");
	FString HttpServer_JoinRoom								= TEXT("/rooms");
	// ======================================================================//

	// ============================== JSON VALUES ==============================//
	FString JsonData_RoomId									= TEXT("id");
	FString JsonData_RoomHost								= TEXT("host");
	FString JsonData_RoomHostReadiness						= TEXT("host_readiness");
	FString JsonData_RoomGuest								= TEXT("guest");
	FString JsonData_RoomGuestReadiness						= TEXT("guest_readiness");
	FString JsonData_RoomStatus								= TEXT("status");
	FString JsonData_RoomCreationTime						= TEXT("created_at");
	// =========================================================================//

	// ============================== Logging ==============================//
	FString LogMessage_RequestRoomsList						= TEXT("Requested all rooms from http server.");
	FString LogMessage_RequestCreateRoom					= TEXT("Requested room creation from http server.");
	FString LogMessage_RequestDeleteRoom					= TEXT("Requested room deletion from http server.");
	FString LogMessage_RequestJoinRoom						= TEXT("Requested room deletion from http server.");

	FString LogMessage_RoomIdEmpty							= TEXT("Unnable to buiold request. RoomID is empty.");

	FString LogMessage_ReceivedRoomsListSuccessfully		= TEXT("Rooms list recieved successfully.");

	FString LogMessage_ReachingServerFail					= TEXT("Failed to reach server!");
	FString LogMessage_DeserializeRoomsListFailed			= TEXT("Failed to parse rooms list JSON.");
	FString LogMessage_DeserializeRoomCreationFailed		= TEXT("Failed to parse created room JSON.");
	FString LogMessage_DeserializeRoomDeletionFailed		= TEXT("Failed to parse deleted room JSON.");
	FString LogMessage_GetFieldsRoomCreationFailed			= TEXT("Failed to get field while parsing created room JSON.");
	FString LogMessage_GetFieldsRoomDeletionFailed			= TEXT("Failed to get field while parsing deleted room JSON.");
	FString LogMessage_GettingCreatedRoomAsObjectFailed		= TEXT("Failed to get room as JSON object.");
	FString LogMessage_GettingDeletedRoomAsObjectFailed		= TEXT("Failed to get room as JSON object.");
	// =====================================================================//
};
