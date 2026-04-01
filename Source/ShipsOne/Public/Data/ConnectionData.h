#pragma once

#include "CoreMinimal.h"
#include "ConnectionData.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(ShLog_Connection, All, All)

UENUM(BlueprintType)
enum class EType : uint8
{
	Type_1,
	Type_2
};