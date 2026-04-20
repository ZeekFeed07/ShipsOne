#pragma once

#include "CoreMinimal.h"
#include "ConnectionData.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(ShLog_Connection, All, All)

USTRUCT(BlueprintType)
struct FRoomInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RoomID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString HostName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HostReadiness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GuestName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool GuestReadiness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RoomStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CreatedTime;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHttpServerRoomsListReceivedMulticast, bool, bSuccess, int32, Code, const TArray<FRoomInfo>&, RoomsList);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnHttpServerRoomsListReceived, bool, bSuccess, int32, Code, const TArray<FRoomInfo>&, RoomsList);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHttpServerRoomCreationMulticast, bool, bSuccess, int32, Code, const FRoomInfo&, NewRoom);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnHttpServerRoomCreation, bool, bSuccess, int32, Code, const FRoomInfo&, NewRoom);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHttpServerRoomDeletionMulticast, bool, bSuccess, int32, Code, const FString&, RoomID);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnHttpServerRoomDeletion, bool, bSuccess, int32, Code, const FString&, RoomID);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHttpServerJoiningRoomMulticast, bool, bSuccess, int32, Code, const FRoomInfo&, JoinedRoom);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnHttpServerJoiningRoom, bool, bSuccess, int32, Code, const FRoomInfo&, JoinedRoom);
