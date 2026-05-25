#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Data/GameplayData.h"
#include "ShGameManager.generated.h"

class AShShipBase;
class AShCellBase;
class AShFieldBase;
class UWidgetsSettingsDataAsset;
class UShipDataAsset;
class UFieldDataAsset;
class UCellDataAsset;

using FTickTask = void (UShGameManager::*)(float);

UCLASS()
class SHIPSONE_API UShGameManager :
	public UWorldSubsystem,
	public FTickableGameObject
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void PostInitialize() override;

	UFUNCTION()
	virtual void SetupControllerRef(APlayerController* PC);

	UFUNCTION()
	virtual void SetupPlayerStateRef(APlayerState* PS);
	
	UFUNCTION(BlueprintCallable)
	virtual void RequestShipCreation(const EShipSize ShipSize = EShipSize::ONE);

	UFUNCTION(BlueprintCallable)
	virtual void RemoveCurrentShip();
	
	UFUNCTION()
	virtual void RotateShipClockwise();

	UFUNCTION()
	virtual void RotateShipCounterClockwise();

	UFUNCTION()
	virtual void StartPlayerFieldCreation();

	UFUNCTION()
	virtual void MakeShip(const EShipSize ShipSize);

	UFUNCTION()
	virtual void PlaceShip();

	UFUNCTION()
	virtual void PullHoveredShip();

private:

	UFUNCTION()
	virtual bool CheckCanCreateShip(EShipSize ShipSize);

	UFUNCTION()
	virtual AShShipBase* SpawnShip(const EShipSize ShipSize);

	UFUNCTION()
	virtual void RemoveShip(AShShipBase*& ShipToRemove);

	UFUNCTION()
	virtual void IncreaseShipNum(EShipSize ShipSize);

	UFUNCTION()
	virtual void DecreaseShipNum(EShipSize ShipSize);

	UFUNCTION()
	virtual void UpdateShipsCollision(bool bEnable);


	// ==================================== Ticks ==================================== //
	
	virtual void TraceUnderCursor(float DeltaSeconds);
	virtual void SnapShipToCursor(float DeltaSeconds);
	virtual void CheckCellUnderCursor(float DeltaSeconds);
	virtual void CheckShipUnderCursor(float DeltaSeconds);

	virtual void AddTickTask(FTickTask TaskToAdd);
	virtual void RemoveTickTask(FTickTask TaskToRemove);

	// =============================================================================== //

	UFUNCTION()
	virtual void CreateShipPlacingWidget();

	bool Debug_CheckShipsNum();

public:
	UPROPERTY(BlueprintAssignable)
	FOnShipsNumStateSignature OnShipsNumStateChange;

	UPROPERTY(BlueprintAssignable)
	FOnGameReadySignature OnGameReady;

	UPROPERTY(BlueprintAssignable)
	FOnShipHandle OnShipCaptured;

	UPROPERTY(BlueprintAssignable)
	FOnShipHandle OnShipReleased;

private:
	// ==================================== System ==================================== //

	TArray<FTickTask> TickTasks;

	UPROPERTY()
	FHitResult CurrentHit;
	
	// ================================================================================ //

	// ==================================== Common ==================================== //

	UPROPERTY()
	TArray<AShShipBase*> CreatedShips;

	UPROPERTY()
	TObjectPtr<APlayerController> ControllerRef;

	UPROPERTY()
	TObjectPtr<APawn> PawnRef;

	UPROPERTY()
	TObjectPtr<APlayerState> StateRef;

	UPROPERTY()
	TMap<EShipSize, int32> ShipsNum;

	UPROPERTY()
	TObjectPtr<AShFieldBase> SelfField;

	UPROPERTY()
	TObjectPtr<AShFieldBase> EnemyField;

	UPROPERTY()
	AShShipBase* CurrentShip = nullptr;

	UPROPERTY()
	AShShipBase* HoveredShip = nullptr;

	UPROPERTY()
	AShCellBase* LastCell = nullptr;

	// ================================================================================ //

	// =================================== Configs =================================== //

	UPROPERTY()
	TObjectPtr<UWidgetsSettingsDataAsset> WidgetConfig;

	UPROPERTY()
	TObjectPtr<UShipDataAsset> ShipConfig;

	UPROPERTY();
	TObjectPtr<UFieldDataAsset> FieldConfig;

	UPROPERTY();
	TObjectPtr<UCellDataAsset> CellConfig;

	UPROPERTY()
	TObjectPtr<UUserWidget> ShipPlacingWidgetRef;

	// =============================================================================== //

	// ==================================== Paths ==================================== //

	const FString WidgetConfigPath	= TEXT("/Game/ShipsOne/Dev/Data/DataAssets/DA_MainWidgetInfo.DA_MainWidgetInfo");
	const FString ShipConfigPath	= TEXT("/Game/ShipsOne/Dev/Data/DataAssets/DA_MainShipsInfo.DA_MainShipsInfo");
	const FString FieldConfigPath	= TEXT("/Game/ShipsOne/Dev/Data/DataAssets/DA_MainFieldInfo.DA_MainFieldInfo");
	const FString CellConfigPath	= TEXT("/Game/ShipsOne/Dev/Data/DataAssets/DA_MainCellInfo.DA_MainCellInfo");

	// =============================================================================== //

	// ==================================== Logging ==================================== //

	FString LogMessage_WorldNotValid					= TEXT("World is not valid.");
	FString LogMessage_ControllerNotValid				= TEXT("Player controller is not valid.");
	FString LogMessage_ControllerNotImplementsInterface	= TEXT("Player controller does not implements necessary interface.");
	FString LogMessage_PlayerStateNotValid				= TEXT("Player state is not valid or not implements interface.");
	FString LogMessage_PawnNotValid						= TEXT("Player Pawn is not valid.");
	FString LogMessage_ShipNotValid						= TEXT("Ship is not valid.");
	FString LogMessage_CursorNotValid					= TEXT("Mouse cursor is not valid.");
	FString LogMessage_ShipSizeNotValid					= TEXT("Ship size is not valid.");
	FString LogMessage_ShipConfigNotValid				= TEXT("Ship config is not valid.");
	FString LogMessage_WidgetConfigNotValid				= TEXT("Widget config is not valid.");
	FString LogMessage_ShipPlacementWidgetNotValid		= TEXT("Ship placement widget is not valid.");
	FString LogMessage_CannotCreateShip					= TEXT("Cannot to create a ship.");
	
	// ================================================================================= //
};
