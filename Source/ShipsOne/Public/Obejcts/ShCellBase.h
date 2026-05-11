#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/GameplayData.h"
#include "ShCellBase.generated.h"

class UCellDataAsset;

UCLASS()
class SHIPSONE_API AShCellBase : public AActor
{
	GENERATED_BODY()
	
public:	

	AShCellBase();
public:	

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	ECellState GetState() { return CurrentState; }

	UFUNCTION()
	virtual void ApplyMesh(UStaticMesh* MeshBody);

	UFUNCTION()
	virtual void ScaleMeshBody(const FVector& Scale);

	UFUNCTION()
	virtual void SetMaterialSource(ECellState StateForMaterial, UMaterialInterface* Material);

	UFUNCTION()
	virtual void UpdateState(ECellState NewState);

	UFUNCTION()
	virtual void UpdateStateTemporary(ECellState TemporaryState);

	UFUNCTION()
	virtual void ResetStateToActual();

protected:

	virtual void BeginPlay() override;

private:

	UFUNCTION()
	virtual void SetMeshMaterial(UMaterialInterface* Material);
private:
	// ==================================== Common ==================================== //

	UPROPERTY(EditDefaultsOnly, Category = "Visual");
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY()
	ECellState CurrentState = ECellState::EMPTY;

	UPROPERTY()
	TMap<ECellState, UMaterialInterface*> StateMaterialCorrespondence;

	// ================================================================================ //
	
	// ==================================== Logging ==================================== //

	FString LogMessage_FindingCellMeshFailure	= TEXT("Unnable to find cell mesh by path.");
	FString LogMessage_MeshPtrNotValid			= TEXT("Mesh pointer not valid while applying.");
	FString LogMessage_MaterialPtrNotValid		= TEXT("Material pointer not valid.");
	
	// ================================================================================= //
};
