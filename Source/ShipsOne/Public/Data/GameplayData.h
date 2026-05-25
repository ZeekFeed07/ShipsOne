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
	SHIPPED = 2,
	FORBIDDEN = 3
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllowCreation);
DECLARE_DYNAMIC_DELEGATE(FOnAllowCreationNotMulticast);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAllowShipCreation, const EShipSize, ShipSize);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAllowShipCreationNotMulticast, const EShipSize, ShipSize);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShipsNumStateSignature, EShipSize, ShipSize, int32, CurrentShipsNum);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShipHandle);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameReadySignature);

#define SH_VALIDATE_RET(Cond, Msg, RetVal) \
    if (!(Cond)) { UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."), \
        *Msg, TEXT(__FUNCTION__), *GetName()); return RetVal; }

#define SH_VALIDATE(Cond, Msg) \
    SH_VALIDATE_RET(Cond, Msg, )