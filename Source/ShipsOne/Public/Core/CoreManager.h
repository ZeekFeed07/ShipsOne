#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CoreManager.generated.h"

UCLASS()
class SHIPSONE_API UCoreManager : public UObject
{
	GENERATED_BODY()
public:
	virtual void InitializeMultiplayer(AController* PlayerController1, AController* PlayerController2);
	virtual void InitializeSoloGameplay(AController* PlayerController);
};
