#pragma once

#include "CoreMinimal.h"
#include "GameplayData.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(ShLog_Gameplay, All, All)

USTRUCT()
struct Fasd
{
	GENERATED_BODY()

	int32 az;
};

UENUM(BlueprintType)
enum class EShipSize : uint8
{
	ONE = 0,
	TWO = 1,
	THREE = 2,
	FOUR = 3
};

UENUM(BlueprintType)
enum class EShipDirection : uint8
{
	TOP = 0,
	RIGHT = 1,
	BOTTOM = 2,
	LEFT = 3
};

UENUM(BlueprintType)
enum class ECellState : uint8
{
	EMPTY = 0,
	DEADZONE = 1,
	SHIPPED = 2
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllowFieldCreation);
DECLARE_DYNAMIC_DELEGATE(FOnAllowFieldCreationNotMulticast);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShipsNumStateSignature, EShipSize, ShipSize, int32, CurrentShipsNum);