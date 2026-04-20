#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/ConnectionData.h"

#include "HttpConnectionInterface.generated.h"

UINTERFACE(MinimalAPI)
class UHttpConnectionInterface : public UInterface
{
	GENERATED_BODY()
};

class SHIPSONE_API IHttpConnectionInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Server Interaction | Requests")
	void RequestRoomsList();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Server Interaction | Requests")
	void RequestCreateNewRoom(const FString& PlayerName);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Server Interaction | Requests")
	void RequestDeleteRoom();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Server Interaction | Requests")
	void RequestJoinToRoom(const FString& PlayerName, const FString& RoomID);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Bindings")
	void BindToRoomsListReceived(const FOnHttpServerRoomsListReceived& Event);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Bindings")
	void UnbindFromRoomsListReceived(const FOnHttpServerRoomsListReceived& Event);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Bindings")
	void BindToRoomCreated(const FOnHttpServerRoomCreation& Event);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Bindings")
	void UnbindFromRoomCreated(const FOnHttpServerRoomCreation& Event);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Bindings")
	void BindToRoomDeleted(const FOnHttpServerRoomDeletion& Event);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Bindings")
	void UnbindFromRoomDeleted(const FOnHttpServerRoomDeletion& Event);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Bindings")
	void BindToRoomJoining(const FOnHttpServerJoiningRoom& Event);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Bindings")
	void UnbindFromRoomJoining(const FOnHttpServerJoiningRoom& Event);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Actions")
	FRoomInfo GetCurrentRoom() const;
};
