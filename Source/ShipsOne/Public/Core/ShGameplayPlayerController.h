#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/GameplayNetworkInterface.h"
#include "Interfaces/GameplayControllerInterface.h"
#include "Data/GameplayData.h"
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
	// ========================= IGameplayController Interface ========================= //

	virtual void ApplyCameraActionsInputContext_Implementation() override;
	virtual void ApplyShipPlacementInputContext_Implementation() override;
	virtual void ApplyShipRemovementInputContext_Implementation() override;
	virtual void RemoveShipPlacementInputContext_Implementation() override;
	virtual void RemoveShipRemovementInputContext_Implementation() override;

	// ================================================================================= //

	// ========================= IGameplayNetwork Interface ========================= //

	virtual void RequestShipCreation_Implementation(EShipSize ShipSize) override;
	virtual void SendFieldInfo_Implementation() override;
	virtual void ApproveFieldCreation_Implementation() override;
	
	// ============================================================================== //

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
	virtual void PullShipInput(const FInputActionValue& Value);

	UFUNCTION()
	virtual void SetupInputMode();
	
	// ================================================================================ //

	// ==================================== Server ====================================//

	UFUNCTION(Server, Reliable)
	void ServerControllerReadyRPC();
	virtual void ServerControllerReadyRPC_Implementation();

	UFUNCTION(Server, Reliable)
	void ServerRequestShipCreation(const EShipSize ShipSize);
	virtual void ServerRequestShipCreation_Implementation(const EShipSize ShipSize);

	UFUNCTION(Server, Reliable)
	void ServerSendFieldInfo();
	virtual void ServerSendFieldInfo_Implementation();

	UFUNCTION(Client, Reliable)
	void ClientCreateFieldApproved();
	virtual void ClientCreateFieldApproved_Implementation();
	
	UFUNCTION(Client, Reliable)
	void ClientCreateShipApproved(const EShipSize ShipSize);
	virtual void ClientCreateShipApproved_Implementation(const EShipSize ShipSize);

	UFUNCTION(Client, Reliable)
	void ClientPlayerCanStart();
	virtual void ClientPlayerCanStart_Implementation();

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
};
