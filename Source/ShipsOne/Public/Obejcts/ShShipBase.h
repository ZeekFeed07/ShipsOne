#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/GameplayData.h"
#include "ShShipBase.generated.h"

class UShipDataAsset;

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
	virtual void RotateClockwise();

	UFUNCTION()
	virtual void SetShipConfig(UShipDataAsset* Asset);

	UFUNCTION()
	virtual EShipSize GetShipSize();

	UFUNCTION()
	virtual EShipDirection GetShipDirection();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	virtual void SetupMesh();

private:

	// ==================================== Common ==================================== //

	UPROPERTY()
	TObjectPtr<UShipDataAsset> ShipConfig;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY()
	EShipSize Size = EShipSize::ONE;

	UPROPERTY()
	EShipDirection Direction = EShipDirection::TOP;

	// ================================================================================ //

	// ==================================== Logging ==================================== //

	FString LogMessage_BodyMeshNotValid		= TEXT("Body mesh is not valid.");
	FString LogMessage_ShipConfigNotValid	= TEXT("Ship config is not valid.");
	FString LogMessage_ShipBodyMeshNotValid = TEXT("Some ship body mesh is not valid.");
	
	// ================================================================================= //

};
