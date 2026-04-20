#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShFieldBase.generated.h"

UCLASS()
class SHIPSONE_API AShFieldBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AShFieldBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;	
};
