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

UENUM()
enum class EShipSize : uint8
{
	ONE = 1,
	TWO = 2,
	THREE = 3,
	FOUR = 4
};

UENUM()
enum class EShipDirection : uint8
{
	TOP = 0,
	RIGHT = 1,
	BOTTOM = 2,
	LEFT = 3
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllowFieldCreation);
DECLARE_DYNAMIC_DELEGATE(FOnAllowFieldCreationNotMulticast);