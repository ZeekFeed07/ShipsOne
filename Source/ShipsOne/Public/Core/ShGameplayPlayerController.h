#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShGameplayPlayerController.generated.h"

UCLASS()
class SHIPSONE_API AShGameplayPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AShGameplayPlayerController();

protected:
	virtual void BeginPlay() override;

	virtual void SetupGameManagers();
};