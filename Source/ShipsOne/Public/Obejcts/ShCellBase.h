#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShCellBase.generated.h"

UCLASS()
class SHIPSONE_API AShCellBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AShCellBase();
public:	
	virtual void Tick(float DeltaTime) override;	
protected:
	virtual void BeginPlay() override;
private:
	
private:
	// ==================================== Common ==================================== //

	UPROPERTY(EditDefaultsOnly, Category = "Visual");
	TObjectPtr<UStaticMeshComponent> Mesh;

	// ================================================================================ //
	
	// ==================================== Logging ==================================== //
	FString LogMessage_FindingCellMeshFailure = TEXT("Unnable to find cell mesh by path.");
	// ================================================================================= //

	// ==================================== Paths ==================================== //
	
	const FString CellMeshPath = TEXT("/Game/ShipsOne/Art/Objects/Cell/SM_Cell.SM_Cell");

	// =============================================================================== //
};
