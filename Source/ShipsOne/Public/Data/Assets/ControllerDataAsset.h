#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ControllerDataAsset.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class SHIPSONE_API UControllerDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> CameraActionsContext;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> ShipPlacementContext;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> CameraRotationInput;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> CameraRotationActivation;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> CameraMovementInput;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> CameraZoomInput;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> ShipPlacementInput;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> ShipRotationInput;
};
