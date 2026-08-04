#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Data/InputData.h"
#include "ShGameplayPlayerController.generated.h"

class UControllerDataAsset;
struct FInputActionValue;

UCLASS()
class SHIPSONE_API AShGameplayPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AShGameplayPlayerController();

public:
	// ==================================== Inputs Handlers ==================================== //
	virtual void RotateCameraAngleInput(const FInputActionValue& Value);
	virtual void MoveCameraInput(const FInputActionValue& Value);
	virtual void ZoomCameraInput(const FInputActionValue& Value);
	virtual void PlaceShipInput(const FInputActionValue& Value);
	virtual void RotateShipInput(const FInputActionValue& Value);
	virtual void PullShipInput(const FInputActionValue& Value);
	// ========================================================================================= //

	// ==================================== Input Mappings ==================================== //
	virtual void ApplyCameraInputMapping();
	virtual void ApplyShipPlacementInputMapping();
	virtual void ApplyShipRemovementInputMapping();

	virtual void RemoveCameraInputMapping();
	virtual void RemoveShipPlacementInputMapping();
	virtual void RemoveShipRemovementInputMapping();
	// ======================================================================================== //
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void SetupGameManagers();
	virtual void SetupInputMode();

	virtual void OnRep_PlayerState() override;
	virtual bool CheckDataReady();
private:
	virtual void LoadControllerConfig();
public:
	UPROPERTY(BlueprintAssignable, Category = "Input")
	FOnControllerInputActionVector2 OnCameraRotationInput;
	UPROPERTY(BlueprintAssignable, Category = "Input")
	FOnControllerInputActionVector2 OnCameraMovementInput;
	UPROPERTY(BlueprintAssignable, Category = "Input")
	FOnControllerInputActionFloat OnCameraZoomInput;
	UPROPERTY(BlueprintAssignable, Category = "Input")
	FOnControllerInputActionBool OnPlaceShipInput;
	UPROPERTY(BlueprintAssignable, Category = "Input")
	FOnControllerInputActionFloat OnRotateShipInput;
	UPROPERTY(BlueprintAssignable, Category = "Input")
	FOnControllerInputActionBool OnPullShipInput;
private:
	TObjectPtr<UControllerDataAsset> ControllerConfig;

	// ==================================== Paths ==================================== //

	const FString ControllerConfigPath = TEXT("/Game/ShipsOne/Configs/DA_MainControllerConfig.DA_MainControllerConfig");

	// =============================================================================== //
};