#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CellDataAsset.generated.h"

UCLASS()
class SHIPSONE_API UCellDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FVector CellSize = FVector(500.f, 500.f, 100.f);

	UPROPERTY(EditAnywhere)
	FVector ScaleMesh = FVector(1.f, 1.f, 1.f);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> MeshRef;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> MaterialRef;
};
