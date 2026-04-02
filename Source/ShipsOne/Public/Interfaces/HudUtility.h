#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "HudUtility.generated.h"

UINTERFACE(MinimalAPI)
class UHudUtility : public UInterface
{
	GENERATED_BODY()
};

class SHIPSONE_API IHudUtility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Initialization")
	void InitMainWidget();
};
