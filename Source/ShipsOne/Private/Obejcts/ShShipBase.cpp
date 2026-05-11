#include "Obejcts/ShShipBase.h"
#include "Data/Assets/ShipDataAsset.h"
#include "Data/Assets/CellDataAsset.h"

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
	auto* SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = SceneComponent;
	
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(SceneComponent);

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

void AShShipBase::SetShipConfig(UShipDataAsset* Asset)
{
	if (IsValid(Asset))
	{
		ShipConfig = Asset;
	}
}

void AShShipBase::SetCellConfig(UCellDataAsset* ConfigPtr)
{
	if (IsValid(ConfigPtr))
	{
		CellConfig = ConfigPtr;
	}
}

void AShShipBase::NormalizeForDirection()
{
	if (!IsValid(CellConfig))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_CellConfigNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	FVector CurrentLocation = FVector::ZeroVector;

	switch (Direction)
	{
	case EShipDirection::TOP:
		CurrentLocation += FVector(CellConfig->CellSize.X / 2.f * (int32)Size, 0.f, 0.f);
		break;
	case EShipDirection::LEFT:
		CurrentLocation += FVector(0.f, CellConfig->CellSize.Y / 2.f * (int32)Size, 0.f);
		break;
	case EShipDirection::BOTTOM:
		CurrentLocation += FVector(-CellConfig->CellSize.X / 2.f * (int32)Size, 0.f, 0.f);
		break;
	case EShipDirection::RIGHT:
		CurrentLocation += FVector(0.f, -CellConfig->CellSize.Y / 2.f * (int32)Size, 0.f);
		break;
	default:
		return;
	}
	Body->AddLocalOffset(CurrentLocation);
}

void AShShipBase::RotateClockwise()
{
	Direction = EShipDirection(((int32)Direction + 1) % 4);
	AddActorWorldRotation({ 0.f, 90.f, 0.f });
}

void AShShipBase::RotateCounterClockwise()
{
	Direction = EShipDirection(((int32)Direction + 3) % 4);
	AddActorWorldRotation({ 0.f, -90.f, 0.f });
}

EShipSize AShShipBase::GetShipSize()
{
	return Size;
}

EShipDirection AShShipBase::GetShipDirection()
{
	return Direction;
}
