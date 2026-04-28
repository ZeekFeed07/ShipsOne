#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/GameplayNetworkInterface.h"
#include "Interfaces/GameplayControllerInterface.h"
#include "ShGameplayPlayerController.generated.h"

class UShGameManager;
class UControllerDataAsset;

struct FInputActionValue;

UCLASS()
class SHIPSONE_API AShGameplayPlayerController :
	public APlayerController,
	public IGameplayControllerInterface,
	public IGameplayNetworkInterface
{
	GENERATED_BODY()

public:
	AShGameplayPlayerController();

public:
	virtual void ApplyCameraActionsInputContext_Implementation() override;
	virtual void ApplyShipPlacementInputContext_Implementation() override;
	virtual void RemoveShipPlacementInputContext_Implementation() override;
protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:

	UFUNCTION()
	virtual void InitManager();

	UFUNCTION()
	virtual void LoadControllerConfig();

	// ==================================== Inputs ==================================== //
	
	UFUNCTION()
	virtual void RotateCameraAngleInput(const FInputActionValue& Value);

	UFUNCTION()
	virtual void MoveCameraInput(const FInputActionValue& Value);

	UFUNCTION()
	virtual void ZoomCameraInput(const FInputActionValue& Value);

	UFUNCTION()
	virtual void PlaceShipInput(const FInputActionValue& Value);

	UFUNCTION()
	virtual void RotateShipInput(const FInputActionValue& Value);

	UFUNCTION()
	virtual void SetupInputMode();
	
	// ================================================================================ //

	// ==================================== Server ====================================//

	UFUNCTION(Server, Reliable)
	void ServerControllerReadyRPC();

	// ================================================================================//
private:
	// ==================================== Common ==================================== //
	
	UPROPERTY();
	TObjectPtr<UShGameManager> Manager;

	UPROPERTY();
	TObjectPtr<UControllerDataAsset> ControllerConfig;

	// ================================================================================ //

	// ==================================== Paths ==================================== //

	const FString ControllerConfigPath = TEXT("/Game/ShipsOne/Dev/Data/DataAssets/DA_MainControllerInfo.DA_MainControllerInfo");

	// =============================================================================== //

	// ==================================== Logging ==================================== //

	FString LogMessage_WorldNotValid				= TEXT("World is not valid.");
	FString LogMessage_GamemodeNotValid				= TEXT("Gamemode is not valid.");
	FString LogMessage_SubsystemNotValid			= TEXT("Manager subsystem is not valid.");
	FString LogMessage_EnhancedSubsystemNotValid	= TEXT("Enhanced player subsystem is not valid.");
	FString LogMessage_EICNotValid					= TEXT("Enhanced input component is not valid.");
	FString LogMessage_PlayerStateNotValid			= TEXT("Player state is not valid.");
	FString LogMessage_ControllerConfigNotValid		= TEXT("Controller config is not valid.");
	FString LogMessage_InputMappingNotValid			= TEXT("Some input mapping context from config is not valid.");
	FString LogMessage_PawnNotValid					= TEXT("Player pawn is not valid.");
	FString LogMessage_PawnNotImplementsInterface	= TEXT("Player pawn does not implement interface.");
	FString LogMessage_CameraRotationInputNotValid	= TEXT("Camera rotation input is not valid.");
	FString LogMessage_CameraMovementInputNotValid	= TEXT("Camera movement input is not valid.");
	FString LogMessage_CameraZoomInputNotValid		= TEXT("Camera zoom input is not valid.");
	FString LogMessage_ShipPlacementInputNotValid	= TEXT("Ship placement input is not valid.");
	FString LogMessage_ShipRotationInputNotValid	= TEXT("Ship rotation input is not valid.");

	// ================================================================================= //
};
