#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/GameplayData.h"
#include "ShFieldBase.generated.h"

class AShShipBase;
class AShCellBase;
class UCellDataAsset;
class UFieldDataAsset;

UCLASS()
class SHIPSONE_API AShFieldBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AShFieldBase();

public:

	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	virtual void PlaceInCenter();
	
	UFUNCTION()
	virtual void PlaceInLeft();

	UFUNCTION()
	virtual void PlaceInRight();

	UFUNCTION()
	virtual void BeginInit();

	UFUNCTION()
	virtual void ShipHoverOn(AShCellBase* CellPtr, AShShipBase* ShipPtr);
	
	UFUNCTION()
	virtual bool PlaceShipOnCell(AShCellBase* CellPtr, AShShipBase* ShipPtr);

	UFUNCTION()
	virtual void RemoveShip(AShShipBase* ShipPtr);

	UFUNCTION()
	void SetFieldConfig(UFieldDataAsset* ConfigPtr);

	UFUNCTION()
	void SetCellConfig(UCellDataAsset* ConfigPtr);

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

private:
	UFUNCTION()
	bool CreateCells();

	UFUNCTION()
	void ClearCells();

	UFUNCTION()
	bool CheckCanPlaceShip(int32 X, int32 Y, EShipDirection Direction, EShipSize ShipSize);

	UFUNCTION()
	void ColorizeAreaPositiveTemporary(int32 X, int32 Y, EShipDirection Direction, EShipSize ShipSize);
	
	UFUNCTION()
	void ColorizeAreaPositiveFinally(int32 X, int32 Y, EShipDirection Direction, EShipSize ShipSize);

	UFUNCTION()
	void ColorizeAreaNegativeTemporary(int32 X, int32 Y, EShipDirection Direction, EShipSize ShipSize);

	UFUNCTION()
	void ResetAllCells();

	UFUNCTION()
	bool IsInBounds(int32 V, int32 Max) { return V >= 0 && V < Max; }

	UFUNCTION()
	bool IsShipNearCell(int32 X, int32 Y);

	UFUNCTION()
	virtual void ConvertID(const int32 ID, int32& X, int32& Y);

	UFUNCTION()
	AShCellBase* GetCell(int32 X, int32 Y);
	
	UFUNCTION()
	void SetCell(int32 X, int32 Y, AShCellBase* NewItem);

	UFUNCTION()
	bool GetCoeffByDir(EShipDirection Dir, int32& OutCoeffX, int32& OutCoeffY);

private:
	// ==================================== Common ==================================== //

	UPROPERTY()
	TArray<AShCellBase*> Field;

	UPROPERTY();
	TObjectPtr<UFieldDataAsset> FieldConfig;

	UPROPERTY();
	TObjectPtr<UCellDataAsset> CellConfig;

	// ================================================================================ //

	// ==================================== Logging ==================================== //

	FString LogMessage_WorldNotValid			= TEXT("World is not valid.");
	FString LogMessage_CellConfigNotValid		= TEXT("Cell config is not valid.");
	FString LogMessage_CellMeshNotValid			= TEXT("Cell mesh is not valid.");
	FString LogMessage_CellMaterialNotValid		= TEXT("Some cell material is not valid.");
	FString LogMessage_FieldConfigNotValid		= TEXT("Field config is not valid.");
	FString LogMessage_ZeroDividing				= TEXT("Dividing by zero.");
	
	// ================================================================================= //
};
