#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/ConnectionData.h"

#include "ControllerUtility.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHttpServerRoomsListReceivedMulticast, bool, bSuccess, int32, Code, const TArray<FRoomInfo>&, RoomsList);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnHttpServerRoomsListReceived, bool, bSuccess, int32, Code, const TArray<FRoomInfo>&, RoomsList);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHttpServerRoomCreationMulticast, bool, bSuccess, int32, Code, const FRoomInfo&, NewRoom);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnHttpServerRoomCreation, bool, bSuccess, int32, Code, const FRoomInfo&, NewRoom);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHttpServerRoomDeletionMulticast, bool, bSuccess, int32, Code, const FString&, RoomID);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnHttpServerRoomDeletion, bool, bSuccess, int32, Code, const FString&, RoomID);

UINTERFACE(MinimalAPI)
class UControllerUtility : public UInterface
{
	GENERATED_BODY()
};

class SHIPSONE_API IControllerUtility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Server Interaction | Requests")
	void RequestRoomsList();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Server Interaction | Requests")
	void RequestCreateNewRoom(const FString& PlayerName);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Server Interaction | Requests")
	void RequestDeleteRoom();

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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Actions")
	FRoomInfo GetCreatedRoom() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Actions")
	void RequestQuitGame();
};
