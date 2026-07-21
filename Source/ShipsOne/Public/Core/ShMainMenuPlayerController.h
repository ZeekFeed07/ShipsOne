#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/HttpConnectionInterface.h"
#include "Data/ConnectionData.h"

#include "ShMainMenuPlayerController.generated.h"

class UHttpControllerBase;

/**
* @brief класс-контроллер для главного меню
*/
UCLASS()
class SHIPSONE_API AShMainMenuPlayerController :
	public APlayerController,
	public IHttpConnectionInterface
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
	
	virtual void RequestJoinToRoom_Implementation(const FString& PlayerName, const FString& RoomID) override;
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

	virtual void BindToRoomJoining_Implementation(const FOnHttpServerJoiningRoom& Event) override;

	virtual void UnbindFromRoomJoining_Implementation(const FOnHttpServerJoiningRoom& Event) override;
	/**
	* @brief Получить созданную комнату
	*
	* @return - информация о созданной комнате
	*/
	virtual FRoomInfo GetCurrentRoom_Implementation() const override;
protected:
	virtual void BeginPlay() override;
	/**
	* @brief Установка режима инпута для главного меню
	*/
	virtual void SetupInputMode();

protected:
	UPROPERTY()
	TObjectPtr<UHttpControllerBase> HttpController;

	// ============================== Logging ==============================//
	FString LogMessage_HudObjectInvalid			= TEXT("HUD is not valid.");
	FString LogMessage_HudInterfaceInvalid		= TEXT("HUD class does not implements interface class.");
	// =====================================================================//

};