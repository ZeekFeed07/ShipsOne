#include "Core/ShGameplayPawn.h"

AShGameplayPawn::AShGameplayPawn()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AShGameplayPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShGameplayPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AShGameplayPawn::BeginPlay()
{
	Super::BeginPlay();
}
