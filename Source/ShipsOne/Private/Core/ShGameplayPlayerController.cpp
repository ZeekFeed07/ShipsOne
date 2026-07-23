#include "Core/ShGameplayPlayerController.h"

AShGameplayPlayerController::AShGameplayPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AShGameplayPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AShGameplayPlayerController::SetupGameManagers()
{

}
