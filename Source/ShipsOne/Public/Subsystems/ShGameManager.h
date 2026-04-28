#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Data/GameplayData.h"
#include "ShGameManager.generated.h"

class AShShipBase;
class AShFieldBase;
class UWidgetsSettingsDataAsset;
class UShipDataAsset;

UCLASS()
class SHIPSONE_API UShGameManager : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void PostInitialize() override;

	UFUNCTION()
	virtual void SetupControllerRef(APlayerController* PC);

	UFUNCTION()
	virtual void SetupPlayerStateRef(APlayerState* PS);
	
	UFUNCTION(BlueprintCallable)
	virtual void MakeShip(const EShipSize ShipSize = EShipSize::ONE);
	
	UFUNCTION()
	virtual void RotateShip();

private:
	UFUNCTION()
	virtual void StartPlayerFieldCreation();

	UFUNCTION()
	virtual bool CheckCanCreateShip(EShipSize ShipSize);

	UFUNCTION()
	virtual AShShipBase* SpawnShip(const EShipSize ShipSize);

	virtual void SnapShipToCursor();

	UFUNCTION()
	virtual void CreateShipPlacingWidget();
private:
	// ==================================== Common ==================================== //
	UPROPERTY()
	TMap<EShipSize, int32> ShipsNum;

	UPROPERTY()
	TArray<AShShipBase*> CreatedShips;

	UPROPERTY()
	bool GameStarted = false;

	UPROPERTY()
	TObjectPtr<APlayerController> ControllerRef;

	UPROPERTY()
	TObjectPtr<APawn> PawnRef;

	UPROPERTY()
	TObjectPtr<APlayerState> StateRef;

	UPROPERTY()
	FOnAllowFieldCreationNotMulticast OnCreateFieldAllowed;

	UPROPERTY()
	TObjectPtr<AShFieldBase> SelfField;

	UPROPERTY()
	TObjectPtr<AShFieldBase> EnemyField;

	UPROPERTY()
	TObjectPtr<AShShipBase> CurrentShip;

	UPROPERTY()
	FTimerHandle SnapShipTimer;

	UPROPERTY()
	float SnapSmoothness = 0.05f;

	UPROPERTY()
	TObjectPtr<UWidgetsSettingsDataAsset> WidgetConfig;

	UPROPERTY()
	TObjectPtr<UShipDataAsset> ShipConfig;

	UPROPERTY()
	TObjectPtr<UUserWidget> ShipPlacingWidgetRef;

	// ================================================================================ //

	// ==================================== Paths ==================================== //

	const FString WidgetConfigPath = TEXT("/Game/ShipsOne/Dev/Data/DataAssets/DA_MainWidgetInfo.DA_MainWidgetInfo");
	const FString ShipConfigPath = TEXT("/Game/ShipsOne/Dev/Data/DataAssets/DA_MainShipsInfo.DA_MainShipsInfo");

	// =============================================================================== //

	// ==================================== Logging ==================================== //

	FString LogMessage_WorldNotValid					= TEXT("World is not valid.");
	FString LogMessage_ControllerNotValid				= TEXT("Player controller is not valid.");
	FString LogMessage_ControllerNotImplementsInterface	= TEXT("Player controller is not valid.");
	FString LogMessage_PlayerStateNotValid				= TEXT("Player state is not valid or not implements interface.");
	FString LogMessage_PawnNotValid						= TEXT("Player Pawn is not valid.");
	FString LogMessage_ShipNotValid						= TEXT("Ship is not valid.");
	FString LogMessage_CursorNotValid					= TEXT("Mouse cursor is not valid.");
	FString LogMessage_ShipSizeNotValid					= TEXT("Ship size is not valid.");
	FString LogMessage_WidgetConfigValid				= TEXT("Widget config is not valid.");
	FString LogMessage_ShipPlacementWidgetNotValid		= TEXT("Widget config is not valid.");
	FString LogMessage_CannotCreateShip					= TEXT("Cannot to create a ship.");
	
	// ================================================================================= //
};
