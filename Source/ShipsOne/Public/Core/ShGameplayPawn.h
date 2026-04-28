#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ShGameplayPawn.generated.h"

UCLASS()
class SHIPSONE_API AShGameplayPawn : public APawn
{
	GENERATED_BODY()

public:
	AShGameplayPawn();
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
};
