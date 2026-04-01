#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ShMainMenuPawn.generated.h"

UCLASS()
class SHIPSONE_API AShMainMenuPawn : public APawn
{
	GENERATED_BODY()

public:
	AShMainMenuPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
