#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/GameplayNetworkInterface.h"
#include "ShGameplayPlayerController.generated.h"

class UShGameManager;

UCLASS()
class SHIPSONE_API AShGameplayPlayerController :
	public APlayerController,
	public IGameplayNetworkInterface
{
	GENERATED_BODY()
public:
	AShGameplayPlayerController();
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void InitManager();

	// ==================================== Server ====================================//

	UFUNCTION(Server, Reliable)
	void ServerControllerReadyRPC();

	// ================================================================================//
private:
	// ==================================== Common ==================================== //
	
	UPROPERTY();
	TObjectPtr<UShGameManager> Manager;

	// ================================================================================ //

	// ==================================== Logging ==================================== //

	FString LogMessage_WorldNotValid		= TEXT("World is not valid.");
	FString LogMessage_GamemodeNotValid		= TEXT("Gamemode is not valid.");
	FString LogMessage_SubsystemNotValid	= TEXT("Manager subsystem is not valid.");
	FString LogMessage_PlayerStateNotValid	= TEXT("Player state is not valid.");

	// ================================================================================= //
};
