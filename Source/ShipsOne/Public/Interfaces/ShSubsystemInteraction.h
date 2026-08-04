#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ShSubsystemInteraction.generated.h"

class AShGameplayPlayerController;
class AShGameplayPlayerState;

UINTERFACE(MinimalAPI)
class UShSubsystemInteraction : public UInterface
{
	GENERATED_BODY()
};

class SHIPSONE_API IShSubsystemInteraction
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void SetupRefs(AShGameplayPlayerController* CurrentPC, AShGameplayPlayerState* CurrentPS);

	UFUNCTION(BlueprintNativeEvent)
	void StartGame();
};
