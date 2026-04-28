#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	virtual void ApplyMesh(UStaticMesh* MeshBody);

	UFUNCTION()
	virtual void ScaleMeshBody(const FVector& Scale);

	UFUNCTION()
	virtual void SetMeshMaterial(UMaterialInterface* Material);
protected:
	virtual void BeginPlay() override;
private:
	
private:
	// ==================================== Common ==================================== //

	UPROPERTY(EditDefaultsOnly, Category = "Visual");
	TObjectPtr<UStaticMeshComponent> Mesh;

	// ================================================================================ //
	
	// ==================================== Logging ==================================== //

	FString LogMessage_FindingCellMeshFailure	= TEXT("Unnable to find cell mesh by path.");
	FString LogMessage_MeshPtrNotValid			= TEXT("Mesh pointer not valid while applying.");
	FString LogMessage_MaterialPtrNotValid		= TEXT("Material pointer not valid while applying.");
	
	// ================================================================================= //
};
