#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Data/GameplayData.h"
#include "ShGameManager.generated.h"

class AShFieldBase;

UCLASS()
class SHIPSONE_API UShGameManager : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION()
	virtual void SetupControllerRef(APlayerController* PC);
	UFUNCTION()
	virtual void SetupPlayerStateRef(APlayerState* PS);

private:
	UFUNCTION()
	virtual void StartPlayerFieldCreation();
private:
	// ==================================== Common ==================================== //

	UPROPERTY()
	bool GameStarted = false;

	UPROPERTY()
	TObjectPtr<APlayerController> ControllerRef;

	UPROPERTY()
	TObjectPtr<APlayerState> StateRef;

	UPROPERTY()
	FOnAllowFieldCreationNotMulticast OnCreateFieldAllowed;

	UPROPERTY()
	TObjectPtr<AShFieldBase> SelfField;

	UPROPERTY()
	TObjectPtr<AShFieldBase> EnemyField;

	// ================================================================================ //

	// ==================================== Logging ==================================== //

	FString LogMessage_ControllerNotValid	= TEXT("Player controller is not valid.");
	FString LogMessage_PlsyerStateNotValid	= TEXT("Player state is not valid or not implements interface.");
	
	// ================================================================================= //
};
