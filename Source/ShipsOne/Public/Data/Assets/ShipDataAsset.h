#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ShipDataAsset.generated.h"

UCLASS()
class SHIPSONE_API UShipDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> Size1Body;

	UPROPERTY(EditAnywhere)
	FVector Scale1 = { 1.0f, 1.0f, 1.0f };

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> Size2Body;
	
	UPROPERTY(EditAnywhere)
	FVector Scale2 = { 1.0f, 1.0f, 1.0f };

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> Size3Body;

	UPROPERTY(EditAnywhere)
	FVector Scale3 = { 1.0f, 1.0f, 1.0f };

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> Size4Body;

	UPROPERTY(EditAnywhere)
	FVector Scale4 = { 1.0f, 1.0f, 1.0f };
};
