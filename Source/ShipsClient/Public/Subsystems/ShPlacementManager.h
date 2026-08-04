#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "ShPlacementManager.generated.h"


UCLASS()
class SHIPSCLIENT_API UShPlacementManager :
    public UTickableWorldSubsystem
{
	GENERATED_BODY()
	using FTickTask = void(UShPlacementManager::*)(float);
public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetShouldTick(bool bInShouldTick = true);
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override;

	virtual void SetControllerRef(class AShGameplayPlayerController* InControllerRef);
	virtual void MakeField();
private:
	virtual void AddTickTask(FTickTask TaskToAdd);
	virtual void RemoveTickTask(FTickTask TaskToRemove);
private:
	TObjectPtr<AShGameplayPlayerController> ControllerRef;

	bool bShouldTick = false;
	TArray<FTickTask> TickTasks;
};
