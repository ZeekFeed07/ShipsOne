#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShFieldBase.generated.h"

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
	UFUNCTION()
	virtual void PlaceInCenter();
	
	UFUNCTION()
	virtual void PlaceInLeft();

	UFUNCTION()
	virtual void PlaceInRight();

	UFUNCTION()
	virtual void BeginInit();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

private:
	UFUNCTION()
	bool CreateCells();

	UFUNCTION()
	void ApplyFieldConfig();

	UFUNCTION()
	void ApplyCellConfig();

	UFUNCTION()
	void ClearCells();

	UFUNCTION()
	AShCellBase* GetCell(int32 X, int32 Y);
	
	UFUNCTION()
	void SetCell(int32 X, int32 Y, AShCellBase* NewItem);

private:
	// ==================================== Common ==================================== //

	UPROPERTY(EditAnywhere, Category = "Data");
	TObjectPtr<UFieldDataAsset> FieldConfig;

	UPROPERTY(EditAnywhere, Category = "Data");
	TObjectPtr<UCellDataAsset> CellConfig;

	TArray<AShCellBase*> Field;

	// ================================================================================ //

	// ==================================== Logging ==================================== //

	FString LogMessage_WorldNotValid			= TEXT("World is not valid.");
	FString LogMessage_CellConfigNotApplied		= TEXT("Cell config does not applied.");
	FString LogMessage_FieldConfigNotApplied	= TEXT("Field config does not applied.");
	
	// ================================================================================= //

	// ==================================== Paths ==================================== //

	const FString FieldConfigPath	= TEXT("/Game/ShipsOne/Dev/Data/DataAssets/DA_MainFieldInfo.DA_MainFieldInfo");
	const FString CellConfigPath	= TEXT("/Game/ShipsOne/Dev/Data/DataAssets/DA_MainCellInfo.DA_MainCellInfo");

	// =============================================================================== //
};
