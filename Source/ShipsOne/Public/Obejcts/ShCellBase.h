#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShCellBase.generated.h"

UCLASS()
class SHIPSONE_API AShCellBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AShCellBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;	
};
