#include "Obejcts/ShCellBase.h"


AShCellBase::AShCellBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AShCellBase::BeginPlay()
{
	Super::BeginPlay();	
}

void AShCellBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}