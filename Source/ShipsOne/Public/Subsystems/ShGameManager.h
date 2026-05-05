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
	virtual void MakeShip(const EShipSize ShipSize = EShipSize::ONE);

	UFUNCTION(BlueprintCallable)
	virtual void RemoveCurrentShip();
	
	UFUNCTION()
	virtual void RotateShipClockwise();

	UFUNCTION()
	virtual void RotateShipCounterClockwise();

private:

	UFUNCTION()
	virtual void StartPlayerFieldCreation();

	UFUNCTION()
	virtual bool CheckCanCreateShip(EShipSize ShipSize);

	UFUNCTION()
	virtual AShShipBase* SpawnShip(const EShipSize ShipSize);

	virtual void RemoveShip(AShShipBase*& ShipToRemove);

	virtual void IncreaseShipNum(EShipSize ShipSize);

	virtual void DecreaseShipNum(EShipSize ShipSize);


	// ==================================== Ticks ==================================== //
	
	virtual void TraceUnderCursor(float DeltaSeconds);
	virtual void SnapShipToCursor(float DeltaSeconds);
	virtual void CheckCellUnderCursor(float DeltaSeconds);

	// =============================================================================== //

	UFUNCTION()
	virtual void CreateShipPlacingWidget();

public:
	UPROPERTY(BlueprintAssignable)
	FOnShipsNumStateSignature OnShipsNumStateChange;
private:
	// ==================================== System ==================================== //

	TArray<FTickTask> TickTasks;

	FHitResult CurrentHit;

	AShCellBase* LastCell;
	
	// ================================================================================ //

	// ==================================== Common ==================================== //

	UPROPERTY()
	TMap<EShipSize, int32> ShipsNum;

	UPROPERTY()
	TArray<AShShipBase*> CreatedShips;

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
	AShShipBase* CurrentShip;

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
	FString LogMessage_ControllerNotImplementsInterface	= TEXT("Player controller is not valid.");
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
