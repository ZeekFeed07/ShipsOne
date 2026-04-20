#include "Obejcts/ShShipBase.h"


AShShipBase::AShShipBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AShShipBase::BeginPlay()
{
	Super::BeginPlay();
}

void AShShipBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

