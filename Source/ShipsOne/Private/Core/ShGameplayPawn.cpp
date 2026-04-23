#include "Core/ShGameplayPawn.h"

AShGameplayPawn::AShGameplayPawn()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AShGameplayPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShGameplayPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShGameplayPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

