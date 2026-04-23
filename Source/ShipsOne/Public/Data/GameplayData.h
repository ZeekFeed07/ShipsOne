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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllowFieldCreation);
DECLARE_DYNAMIC_DELEGATE(FOnAllowFieldCreationNotMulticast);