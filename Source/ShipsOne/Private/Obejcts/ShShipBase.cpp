#include "Obejcts/ShShipBase.h"
#include "Data/Assets/ShipDataAsset.h"

AShShipBase::AShShipBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SetupMesh();
}

void AShShipBase::BeginPlay()
{
	Super::BeginPlay();
}

void AShShipBase::SetupMesh()
{
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	RootComponent = Body;

	Body->AddLocalRotation({0.f, 90.f, 0.f});
	Body->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void AShShipBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShShipBase::SetShipSize(const EShipSize ShipSize)
{
	if (!IsValid(ShipConfig))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_ShipConfigNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	if (!IsValid(ShipConfig->Size1Body) ||
		!IsValid(ShipConfig->Size2Body) ||
		!IsValid(ShipConfig->Size3Body) ||
		!IsValid(ShipConfig->Size4Body))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_ShipBodyMeshNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	switch (ShipSize)
	{
	case EShipSize::ONE:
		Body->SetStaticMesh(ShipConfig->Size1Body);
		Body->SetRelativeScale3D(ShipConfig->Scale1);
		break;
	case EShipSize::TWO:
		Body->SetStaticMesh(ShipConfig->Size2Body);
		Body->SetRelativeScale3D(ShipConfig->Scale2);
		break;
	case EShipSize::THREE:
		Body->SetStaticMesh(ShipConfig->Size3Body);
		Body->SetRelativeScale3D(ShipConfig->Scale3);
		break;
	case EShipSize::FOUR:
		Body->SetStaticMesh(ShipConfig->Size4Body);
		Body->SetRelativeScale3D(ShipConfig->Scale4);
		break;
	default:
		break;
	}

	Size = ShipSize;
}

void AShShipBase::SetShipDirection(const EShipDirection ShipDirection)
{
	Direction = ShipDirection;
}

void AShShipBase::RotateClockwise()
{
	Direction = EShipDirection(((int32)Direction + 1) % 4);
	AddActorWorldRotation({ 0.f, 90.f, 0.f });
}

void AShShipBase::SetShipConfig(UShipDataAsset* Asset)
{
	ShipConfig = Asset;
}

EShipSize AShShipBase::GetShipSize()
{
	return Size;
}

EShipDirection AShShipBase::GetShipDirection()
{
	return Direction;
}

