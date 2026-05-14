#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "GameplayNetworkInterface.generated.h"

enum class EShipSize : uint8;

UINTERFACE(MinimalAPI)
class UGameplayNetworkInterface : public UInterface
{
	GENERATED_BODY()
};

class SHIPSONE_API IGameplayNetworkInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void RequestShipCreation(EShipSize ShipSize);
	
	UFUNCTION(BlueprintNativeEvent)
	void ApproveFieldCreation();
};
