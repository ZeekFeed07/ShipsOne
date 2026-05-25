#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/GameplayData.h"
#include "ShShipBase.generated.h"

class UShipDataAsset;
class UCellDataAsset;

UCLASS()
class SHIPSONE_API AShShipBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AShShipBase();

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void SetShipSize(const EShipSize ShipSize);

	UFUNCTION()
	virtual void SetShipDirection(const EShipDirection ShipDirection);

	UFUNCTION()
	virtual void SetShipConfig(UShipDataAsset* Asset);

	UFUNCTION()
	virtual void SetCellConfig(UCellDataAsset* Asset);

	UFUNCTION()
	virtual void SetPlacedCellID(const int32 X, const int32 Y);

	UFUNCTION()
	virtual void SetCollision(bool bCollision);

	UFUNCTION()
	virtual void NormalizeForDirection();

	UFUNCTION()
	virtual void RotateClockwise();

	UFUNCTION()
	virtual void RotateCounterClockwise();

	UFUNCTION()
	virtual EShipSize GetShipSize();

	UFUNCTION()
	virtual EShipDirection GetShipDirection();

	UFUNCTION()
	virtual void GetPlacedCellID(int32& X, int32& Y);

	UFUNCTION()
	virtual void UpdateOutline(bool bEnable);

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	virtual void SetupMesh();

private:

	// ==================================== Common ==================================== //

	UPROPERTY()
	TObjectPtr<UShipDataAsset> ShipConfig;

	UPROPERTY();
	TObjectPtr<UCellDataAsset> CellConfig;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY();
	UMaterialInterface* DynamicMat = nullptr;

	UPROPERTY()
	EShipSize Size = EShipSize::ONE;

	UPROPERTY()
	EShipDirection Direction = EShipDirection::TOP;

	UPROPERTY()
	int32 PlacedX = INDEX_NONE;

	UPROPERTY()
	int32 PlacedY = INDEX_NONE;

	// ================================================================================ //

	// ==================================== Logging ==================================== //

	FString LogMessage_BodyMeshNotValid				= TEXT("Body mesh is not valid.");
	FString LogMessage_ShipConfigNotValid			= TEXT("Ship config is not valid.");
	FString LogMessage_CellConfigNotValid			= TEXT("Cell config is not valid.");
	FString LogMessage_ShipBodyMeshNotValid			= TEXT("Some ship body mesh is not valid.");
	FString LogMessage_ShipOutlineMaterialNotValid	= TEXT("Some ship body mesh is not valid.");
	
	// ================================================================================= //

};
