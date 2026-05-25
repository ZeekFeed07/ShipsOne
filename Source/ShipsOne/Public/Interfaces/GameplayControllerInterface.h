#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/GameplayData.h"

#include "GameplayControllerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UGameplayControllerInterface : public UInterface
{
	GENERATED_BODY()
};

class SHIPSONE_API IGameplayControllerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void ApplyCameraActionsInputContext();

	UFUNCTION(BlueprintNativeEvent)
	void ApplyShipPlacementInputContext();

	UFUNCTION(BlueprintNativeEvent)
	void ApplyShipRemovementInputContext();

	UFUNCTION(BlueprintNativeEvent)
	void RemoveShipPlacementInputContext();

	UFUNCTION(BlueprintNativeEvent)
	void RemoveShipRemovementInputContext();
};
