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
	FString CreatedTime;
};