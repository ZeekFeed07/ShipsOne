#include "Core/ShMainMenuPawn.h"

AShMainMenuPawn::AShMainMenuPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = false;
}

void AShMainMenuPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AShMainMenuPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

