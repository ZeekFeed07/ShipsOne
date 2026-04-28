#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "GameplayPawnInterface.generated.h"

UINTERFACE(MinimalAPI)
class UGameplayPawnInterface : public UInterface
{
	GENERATED_BODY()
};

class SHIPSONE_API IGameplayPawnInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void RotateCameraAngle2D(const FVector2D& Angle);

	UFUNCTION(BlueprintNativeEvent)
	void MoveCamera2D(const FVector2D& Angle);

	UFUNCTION(BlueprintNativeEvent)
	void ZoomCamera(float Value);
};
