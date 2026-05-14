#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ShGameplayGameMode.generated.h"

class UCoreManager;
class UAIBot;

USTRUCT()
struct FPlayerReadiness
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<APlayerController> PCRef = nullptr;

	UPROPERTY()
	bool bReady = false;

	bool operator==(const APlayerController* InPC) const
	{
		return InPC == PCRef;
	}
};

UCLASS()
class SHIPSONE_API AShGameplayGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AShGameplayGameMode();

public:
	UFUNCTION()
	virtual void PlayerReady(APlayerController* PC);
private:
	UFUNCTION()
	virtual void SendPrepareField(APlayerController* PC);

	virtual bool InitializeGameManagerMulti();
	virtual bool InitializeGameManagerSolo();
private:
	// ==================================== Common ==================================== //

	UPROPERTY();
	TObjectPtr<UCoreManager> GameManager;

	UPROPERTY();
	TObjectPtr<UAIBot> Bot;

	UPROPERTY()
	TArray<FPlayerReadiness> PlayersReadiness;

	// ================================================================================ //

	// ==================================== Logging ==================================== //

	FString LogMessage_ControllerNotValid				= TEXT("Player controller is not valid.");
	FString LogMessage_ControllerNotImplementsInterface = TEXT("Player controller does not implements necessary interface.");
	FString LogMessage_StateNotValid					= TEXT("Player state is not valid or not implements interface.");
	FString LogMessage_ControllerAlreadySet				= TEXT("Player controller is already set.");
	FString LogMessage_MoreThan2Detected				= TEXT("More than 2 players detected.");
	FString LogMessage_GameManagerInitFailure			= TEXT("Game manager initialization failed.");

	// ================================================================================= //

};
	