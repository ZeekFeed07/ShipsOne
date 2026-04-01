#include "Core/ShMainMenuPawn.h"

AShMainMenuPawn::AShMainMenuPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(false);
}

void AShMainMenuPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AShMainMenuPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

