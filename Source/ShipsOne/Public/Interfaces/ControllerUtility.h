#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/ConnectionData.h"

#include "ControllerUtility.generated.h"

UINTERFACE(MinimalAPI)
class UControllerUtility : public UInterface
{
	GENERATED_BODY()
};

class SHIPSONE_API IControllerUtility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Actions")
	void RequestQuitGame();
};
