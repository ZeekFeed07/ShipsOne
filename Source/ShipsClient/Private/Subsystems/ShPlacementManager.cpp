#include "Subsystems/ShPlacementManager.h"
#include "Data/GameplayData.h"
#include "Core/ShGameplayPlayerController.h"
#include "GridModel.h"

void UShPlacementManager::Tick(float DeltaTime)
{
}

void UShPlacementManager::SetShouldTick(bool bInShouldTick)
{
	bShouldTick = bInShouldTick;
}

TStatId UShPlacementManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UShMatchSubsystem, STATGROUP_Tickables);
}

bool UShPlacementManager::IsTickable() const
{
	return Super::IsTickable() && bShouldTick;
}

void UShPlacementManager::SetControllerRef(AShGameplayPlayerController* InControllerRef)
{
	ControllerRef = InControllerRef;
}

void UShPlacementManager::MakeField()
{
}

void UShPlacementManager::AddTickTask(FTickTask TaskToAdd)
{
    if (!TickTasks.Contains(TaskToAdd))
    {
        TickTasks.Add(TaskToAdd);
    }
}

void UShPlacementManager::RemoveTickTask(FTickTask TaskToRemove)
{
    if (TickTasks.Contains(TaskToRemove))
    {
        TickTasks.Remove(TaskToRemove);
    }
}