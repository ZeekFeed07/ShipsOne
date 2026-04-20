#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShShipBase.generated.h"

UCLASS()
class SHIPSONE_API AShShipBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AShShipBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
