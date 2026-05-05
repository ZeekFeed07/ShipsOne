#include "Obejcts/ShFieldBase.h"
#include "Obejcts/ShShipBase.h"
#include "Obejcts/ShCellBase.h"
#include "Data/Assets/FieldDataAsset.h"
#include "Data/Assets/CellDataAsset.h"

AShFieldBase::AShFieldBase()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Base = CreateDefaultSubobject<USceneComponent>(TEXT("Body"));
	RootComponent = Base;
}

void AShFieldBase::PlaceInCenter()
{
	SetActorLocation({0.f, 0.f, 0.f});
}

void AShFieldBase::PlaceInLeft()
{
	if (!IsValid(FieldConfig))
	{
		UE_LOG(ShLog_Gameplay,
			Error,
			TEXT("%s Func: %s. Obj: %s"),
			*LogMessage_FieldConfigNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	float FieldX = FieldConfig->FieldSizeX * CellConfig->CellSize.X / 2.f;
	float FieldY = FieldConfig->FieldSizeY * CellConfig->CellSize.Y / 2.f;
	SetActorLocation({ FieldX + FieldConfig->CenterGap, FieldY, 0.f });
}

void AShFieldBase::PlaceInRight()
{
	if (!IsValid(FieldConfig))
	{
		UE_LOG(ShLog_Gameplay,
			Error,
			TEXT("%s Func: %s. Obj: %s"),
			*LogMessage_FieldConfigNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	float FieldX = FieldConfig->FieldSizeX * CellConfig->CellSize.X / 2.f;
	float FieldY = FieldConfig->FieldSizeY * CellConfig->CellSize.Y / 2.f;
	SetActorLocation({ FieldX - FieldConfig->CenterGap, FieldY, 0.f });
}

void AShFieldBase::BeginInit()
{
	CreateCells();
}

void AShFieldBase::ShipHoverOn(AShCellBase* CellPtr, AShShipBase* ShipPtr)
{
	if (!IsValid(ShipPtr) || !IsValid(CellPtr) || !Field.Contains(CellPtr)) return;
	if (!IsValid(FieldConfig))
	{
		UE_LOG(ShLog_Gameplay,
			Error,
			TEXT("%s Func: %s. Obj: %s"),
			*LogMessage_FieldConfigNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	if (FieldConfig->FieldSizeX == 0)
	{
		UE_LOG(ShLog_Gameplay,
			Error,
			TEXT("%s Func: %s. Obj: %s"),
			*LogMessage_ZeroDividing,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	int32 RawID = Field.Find(CellPtr);

	const int32 X = RawID / FieldConfig->FieldSizeY;
	const int32 Y = RawID % FieldConfig->FieldSizeY;

	const EShipDirection Dir = ShipPtr->GetShipDirection();
	const EShipSize Size = ShipPtr->GetShipSize();

	CheckCanPlaceShip(X, Y, Dir, Size)
		? ColorizeAreaPositiveTemporary(X, Y, Dir, Size)
		: ColorizeAreaNegativeTemporary(X, Y, Dir, Size);
}

void AShFieldBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShFieldBase::BeginPlay()
{
	Super::BeginPlay();
}

void AShFieldBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ClearCells();
	Super::EndPlay(EndPlayReason);
}

bool AShFieldBase::CreateCells()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(ShLog_Gameplay,
			Error,
			TEXT("%s Func: %s. Obj: %s"),
			*LogMessage_WorldNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return false;
	}
	if (!IsValid(FieldConfig))
	{
		UE_LOG(ShLog_Gameplay,
			Error,
			TEXT("%s Func: %s. Obj: %s"),
			*LogMessage_FieldConfigNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return false;
	}
	if (!IsValid(CellConfig))
	{
		UE_LOG(ShLog_Gameplay,
			Error,
			TEXT("%s Func: %s. Obj: %s"),
			*LogMessage_CellConfigNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return false;
	}
	if (!IsValid(CellConfig->MeshRef))
	{
		UE_LOG(ShLog_Gameplay,
			Error,
			TEXT("%s Func: %s. Obj: %s"),
			*LogMessage_CellMeshNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return false;
	}
	if (!IsValid(CellConfig->MaterialRef))
	{
		UE_LOG(ShLog_Gameplay,
			Error,
			TEXT("%s Func: %s. Obj: %s"),
			*LogMessage_CellMaterialNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return false;
	}

	ClearCells();

	Field.SetNum(FieldConfig->FieldSizeX * FieldConfig->FieldSizeY);

	FVector Position = FVector::Zero();
	AShCellBase* CellPtr = nullptr;

	float FieldHalfX = FieldConfig->FieldSizeX * CellConfig->CellSize.X / 2.f;
	float FieldHalfY = FieldConfig->FieldSizeY * CellConfig->CellSize.Y / 2.f;

	float CellHalfX = CellConfig->CellSize.X / 2.f;
	float CellHalfY = CellConfig->CellSize.Y / 2.f;

	for (int32 i = 0; i < FieldConfig->FieldSizeX; ++i)
	{
		for (int32 j = 0; j < FieldConfig->FieldSizeY; ++j)
		{
			Position = GetActorLocation()
				-FVector(FieldHalfX, FieldHalfY, 0)
				+FVector(CellConfig->CellSize.X * i + CellHalfX, CellHalfY + CellConfig->CellSize.Y * j, 0);
			CellPtr = World->SpawnActor<AShCellBase>(Position, {});

			checkf(IsValid(CellPtr), TEXT("Failed to spawn cell at [%d][%d]"), i, j);
			
			CellPtr->ApplyMesh(CellConfig->MeshRef);
			CellPtr->ScaleMeshBody(CellConfig->ScaleMesh);
			CellPtr->SetMeshMaterial(CellConfig->MaterialRef);

			SetCell(i, j, CellPtr);
		}
	}

	return true;
}

void AShFieldBase::SetFieldConfig(UFieldDataAsset* ConfigPtr)
{
	if (IsValid(ConfigPtr))
	{
		FieldConfig = ConfigPtr;
	}
}

void AShFieldBase::SetCellConfig(UCellDataAsset* ConfigPtr)
{
	if (IsValid(ConfigPtr))
	{
		CellConfig = ConfigPtr;
	}
}

void AShFieldBase::ClearCells()
{
	for (auto& Cell : Field)
	{
		if (IsValid(Cell))
		{
			Cell->Destroy();
		}
	}
	Field.Empty();
}

bool AShFieldBase::CheckCanPlaceShip(int32 X, int32 Y, EShipDirection Direction, EShipSize ShipSize)
{
	if (!IsValid(FieldConfig))
	{
		UE_LOG(ShLog_Gameplay,
			Error,
			TEXT("%s Func: %s. Obj: %s"),
			*LogMessage_FieldConfigNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return false;
	}

	int32 DirIntX;
	int32 DirIntY;

	switch (Direction)
	{
	case EShipDirection::TOP:
		DirIntX = 0;
		DirIntY = 1;
		break;
	case EShipDirection::RIGHT:
		DirIntX = -1;
		DirIntY = 0;
		break;
	case EShipDirection::BOTTOM:
		DirIntX = 0;
		DirIntY = -1;
		break;
	case EShipDirection::LEFT:
		DirIntX = 1;
		DirIntY = 0;
		break;
	default:
		return false;
	}

	int32 NewPosX = X + (uint8)ShipSize * DirIntX;
	int32 NewPosY = Y + (uint8)ShipSize * DirIntY;

	return	(NewPosX > 0 && NewPosX < FieldConfig->FieldSizeX) &&
			(NewPosY > 0 && NewPosY < FieldConfig->FieldSizeY);
}

void AShFieldBase::ColorizeAreaPositiveTemporary(int32 X, int32 Y, EShipDirection Direction, EShipSize ShipSize)
{
}

void AShFieldBase::ColorizeAreaPositiveFinally(int32 X, int32 Y, EShipDirection Direction, EShipSize ShipSize)
{
}

void AShFieldBase::ColorizeAreaNegativeTemporary(int32 X, int32 Y, EShipDirection Direction, EShipSize ShipSize)
{
}

void AShFieldBase::ColorizeAreaNegativeFinally(int32 X, int32 Y, EShipDirection Direction, EShipSize ShipSize)
{
}

AShCellBase* AShFieldBase::GetCell(int32 X, int32 Y)
{
	checkf(
		(X >= 0 && X < FieldConfig->FieldSizeX) && (Y >= 0 && Y < FieldConfig->FieldSizeY),
		TEXT("Accessing field out of bounds. X = %d, Y = %d, when XSize = %d and YSize = %d"),
		X, Y, FieldConfig->FieldSizeX, FieldConfig->FieldSizeY
	);
	return Field[X * FieldConfig->FieldSizeY + Y];
}

void AShFieldBase::SetCell(int32 X, int32 Y, AShCellBase* NewItem)
{
	checkf(
		(X >= 0 && X < FieldConfig->FieldSizeX) && (Y >= 0 && Y < FieldConfig->FieldSizeY),
		TEXT("Accessing field out of bounds. X = %d, Y = %d, when XSize = %d and YSize = %d"),
		X, Y, FieldConfig->FieldSizeX, FieldConfig->FieldSizeY
	);
	Field[X * FieldConfig->FieldSizeY + Y] = NewItem;
}