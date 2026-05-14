#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/GameplayData.h"

#include "StateUtilityInterface.generated.h"

UINTERFACE(MinimalAPI)
class UStateUtilityInterface : public UInterface
{
	GENERATED_BODY()
};

class SHIPSONE_API IStateUtilityInterface
{
	GENERATED_BODY()

public:
	//UFUNCTION(BlueprintNativeEvent)
	//void AllowToCreateField();

	//UFUNCTION(BlueprintNativeEvent)
	//void BindToFieldCreationPermission(const FOnAllowCreationNotMulticast& Event);

	//UFUNCTION(BlueprintNativeEvent)
	//void UnbindFromFieldCreationPermission(const FOnAllowCreationNotMulticast& Event);

	//UFUNCTION(BlueprintNativeEvent)
	//void AllowToCreateShip();
};
