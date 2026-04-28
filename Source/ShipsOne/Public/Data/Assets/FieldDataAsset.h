#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FieldDataAsset.generated.h"

UCLASS()
class SHIPSONE_API UFieldDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float CenterGap = 200.f;

	UPROPERTY(EditAnywhere)
	int32 FieldSizeX = 10;

	UPROPERTY(EditAnywhere)
	int32 FieldSizeY = 10;
};
