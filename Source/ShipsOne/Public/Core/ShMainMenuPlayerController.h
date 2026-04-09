#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HttpModule.h"
#include "Interfaces/ControllerUtility.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Data/ConnectionData.h"

#include "ShMainMenuPlayerController.generated.h"

/**
* @brief класс-контроллер для главного меню
*/
UCLASS()
class SHIPSONE_API AShMainMenuPlayerController :
	public APlayerController,
	public IControllerUtility
{
	GENERATED_BODY()
public:
	AShMainMenuPlayerController();
public:
	/**
	* @brief Отправка запроса на http сервер на получние списка комнат
	*/
	virtual void RequestRoomsList_Implementation() override;
	/**
	* @brief Отправка запроса на http сервер на создание новой комнаты
	* 
	* @param PlayerName - имя игрока 
	*/
	virtual void RequestCreateNewRoom_Implementation(const FString& PlayerName) override;
	/**
	* @brief Удалить текущую комнату созданную комнату
	*/
	virtual void RequestDeleteRoom_Implementation() override;
	/**
	* @brief Подписка на обновление списка комнат
	* 
	* @param Event - коллбэк 
	*/
	virtual void BindToRoomsListReceived_Implementation(const FOnHttpServerRoomsListReceived& Event) override;
	/**
	* @brief Отписка от обновления списка комнат
	* 
	* @param Event - отписываемый коллбэк 
	*/
	virtual void UnbindFromRoomsListReceived_Implementation(const FOnHttpServerRoomsListReceived& Event) override;
	/**
	* @brief Подписка на создание новой комнаты
	*
	* @param Event - коллбэк
	*/
	virtual void BindToRoomCreated_Implementation(const FOnHttpServerRoomCreation& Event) override;
	/**
	* @brief Отписка от события создания новой комнаты
	*
	* @param Event - отписываемый коллбэк
	*/
	virtual void UnbindFromRoomCreated_Implementation(const FOnHttpServerRoomCreation& Event) override;
	/**
	* @brief Подписка на удаление текущей комнаты
	*
	* @param Event - коллбэк
	*/
	virtual void BindToRoomDeleted_Implementation(const FOnHttpServerRoomDeletion& Event) override;
	/**
	* @brief Отписка от события удаления текущей комнаты
	*
	* @param Event - отписываемый коллбэк
	*/
	virtual void UnbindFromRoomDeleted_Implementation(const FOnHttpServerRoomDeletion& Event) override;
	/**
	* @brief Получить созданную комнату
	*
	* @return - информация о созданной комнате
	*/
	virtual FRoomInfo GetCreatedRoom_Implementation() const override;
	/**
	* @brief Запрос на выход из игры
	*/
	virtual void RequestQuitGame_Implementation() override;
protected:
	virtual void BeginPlay() override;
	/**
	* @brief Установка режима инпута для главного меню
	*/
	virtual void SetupInputMode();
	/**
	* @brief Получение ответа от http сервера на запрос о получении списка комнат
	* 
	* @param Request - отправленный запрос
	* @param Response - полученный ответ
	* @param bSuccess - успешость запроса
	*/
	virtual void OnHttpServerRoomsListReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);
	/**
	* @brief Получение ответа от http сервера на запрос о создании новой комнаты
	*
	* @param Request - отправленный запрос
	* @param Response - полученный ответ
	* @param bSuccess - успешость запроса
	*/
	virtual void OnHttpServerRoomCreated(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);
	/**
	* @brief Получение ответа от http сервера на запрос об удалении текущей комнаты
	*
	* @param Request - отправленный запрос
	* @param Response - полученный ответ
	* @param bSuccess - успешость запроса
	*/
	virtual void OnHttpServerRoomDeleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);
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
private:
	FString HttpServer_Link = "http://127.0.0.1:8080";

	FRoomInfo CreatedRoom;

	// ============================== REST API ==============================//
	FString HttpServer_AllRoomsRoute					= TEXT("/rooms");
	FString HttpServer_CreateRoom						= TEXT("/rooms");
	FString HttpServer_DeleteRoom						= TEXT("/rooms");
		
	// ======================================================================//

	// ============================== JSON VALUES ==============================//
	FString JsonData_RoomId								= TEXT("id");
	FString JsonData_RoomHost							= TEXT("host");
	FString JsonData_RoomCreationTime					= TEXT("created_at");
	// =========================================================================//

	// ============================== Logging ==============================//
	FString LogMessage_HudObjectInvalid					= TEXT("HUD is not valid.");
	FString LogMessage_HudInterfaceInvalid				= TEXT("HUD class does not support interface class.");

	FString LogMessage_RequestRoomsList					= TEXT("Requested all rooms from http server.");
	FString LogMessage_RequestCreateRoom				= TEXT("Requested room creation from http server.");
	FString LogMessage_RequestDeleteRoom				= TEXT("Requested room deletion from http server.");

	FString LogMessage_RoomIdEmpty						= TEXT("Unnable to buiold request. RoomID is empty.");

	FString LogMessage_ReceivedRoomsListSuccessfully	= TEXT("Rooms list recieved successfully.");

	FString LogMessage_ReachingServerFail				= TEXT("Failed to reach server!");
	FString LogMessage_DeserializeRoomsListFailed		= TEXT("Failed to parse rooms list JSON.");
	FString LogMessage_DeserializeRoomCreationFailed	= TEXT("Failed to parse created room JSON.");
	FString LogMessage_DeserializeRoomDeletionFailed	= TEXT("Failed to parse deleted room JSON.");
	FString LogMessage_GetFieldsRoomCreationFailed		= TEXT("Failed to get field while parsing created room JSON.");
	FString LogMessage_GetFieldsRoomDeletionFailed		= TEXT("Failed to get field while parsing deleted room JSON.");
	FString LogMessage_GettingCreatedRoomAsObjectFailed	= TEXT("Failed to get room as JSON object.");
	FString LogMessage_GettingDeletedRoomAsObjectFailed	= TEXT("Failed to get room as JSON object.");
	// =====================================================================//
};