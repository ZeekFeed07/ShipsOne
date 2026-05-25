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

};
	