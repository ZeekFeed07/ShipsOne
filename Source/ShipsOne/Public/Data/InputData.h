#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "InputData.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnControllerInputActionBool, const bool, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnControllerInputActionFloat, const float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnControllerInputActionVector2, const FVector2D, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnControllerInputActionVector3, const FVector, Value);

DECLARE_LOG_CATEGORY_EXTERN(ShLog_InputSystem, All, All);

USTRUCT()
struct FInputDummy { GENERATED_BODY() };