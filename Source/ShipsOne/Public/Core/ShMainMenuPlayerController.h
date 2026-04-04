#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Data/ConnectionData.h"
#include "ShMainMenuPlayerController.generated.h"

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnHttpServerRoomsListRecieved, bool, bSuccess, int32, Code, TArray<FRoomInfo>, RoomsList);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHttpServerRoomsListRecievedMulticast, bool, bSuccess, int32, Code, TArray<FRoomInfo>, RoomsList);

UCLASS()
class SHIPSONE_API AShMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AShMainMenuPlayerController();
public:
	UFUNCTION(BlueprintCallable, Category = "Quit")
	virtual void RequestQuitGame();

	UFUNCTION(BlueprintCallable, Category = "Server Interaction")
	virtual void RequestRoomsList(const FOnHttpServerRoomsListRecieved& Callback);

	FOnHttpServerRoomsListRecievedMulticast OnRoomsListRecieved;
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputMode();
	virtual void OnHttpServerRoomsListRecieved(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);
private:
	void SendCheckRequest();
	void OnResponseConfirmed(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

private:
	FString HttpServer_Link = "http://localhost:8080";
	FString HttpServer_AllRoomsRoute = "/rooms";

	FString RequestRoomsListLogMessage = TEXT("Requested all rooms from http server.");
};