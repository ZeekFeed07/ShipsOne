#include "Obejcts/ShFieldBase.h"


AShFieldBase::AShFieldBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AShFieldBase::BeginPlay()
{
	Super::BeginPlay();
}

void AShFieldBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

