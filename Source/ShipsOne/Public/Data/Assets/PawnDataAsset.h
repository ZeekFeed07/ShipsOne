#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PawnDataAsset.generated.h"

UCLASS(BlueprintType)
class SHIPSONE_API UPawnDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float CameraMovementSpeed = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float CameraRotationSpeed = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float CameraDistance = 5000.f;
};
