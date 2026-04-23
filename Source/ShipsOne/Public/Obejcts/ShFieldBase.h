#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShFieldBase.generated.h"

class AShCellBase;

UCLASS()
class SHIPSONE_API AShFieldBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AShFieldBase();

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
	bool CreateCells();

	void ClearCells();

	AShCellBase* GetCell(int32 X, int32 Y);
	void SetCell(int32 X, int32 Y, AShCellBase* NewItem);

private:
	// ==================================== Common ==================================== //

	UPROPERTY(EditAnywhere)
	FVector CellSize = FVector(100.f, 100.f, 10.f);

	UPROPERTY(EditAnywhere)
	float CenterGap = 200.f;

	UPROPERTY(EditAnywhere)
	int32 FieldSizeX = 10;

	UPROPERTY(EditAnywhere)
	int32 FieldSizeY = 10;

	TArray<AShCellBase*> Field;

	// ================================================================================ //

	// ==================================== Logging ==================================== //

	FString LogMessage_WorldNotValid = TEXT("World is not valid.");
	
	// ================================================================================= //
};
