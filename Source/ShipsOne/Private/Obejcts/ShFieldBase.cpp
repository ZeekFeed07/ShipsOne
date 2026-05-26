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
	SH_VALIDATE(IsValid(FieldConfig), LogMessage::FieldConfigNotValid);

	float FieldX = FieldConfig->FieldSizeX * CellConfig->CellSize.X / 2.f;
	float FieldY = FieldConfig->FieldSizeY * CellConfig->CellSize.Y / 2.f;
	SetActorLocation({ FieldX + FieldConfig->CenterGap, FieldY, 0.f });
}

void AShFieldBase::PlaceInRight()
{
	SH_VALIDATE(IsValid(FieldConfig), LogMessage::FieldConfigNotValid);

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
	if (!IsValid(ShipPtr) || !IsValid(CellPtr) || !Field.Contains(CellPtr))
	{
		ResetAllCells();
		return;
	}

	int32 RawID = Field.Find(CellPtr);

	int32 X = INDEX_NONE, Y = INDEX_NONE;
	ConvertID(RawID, X, Y);

	const EShipDirection Dir = ShipPtr->GetShipDirection();
	const EShipSize Size = ShipPtr->GetShipSize();

	CheckCanPlaceShip(X, Y, Dir, Size)
		? ColorizeAreaPositiveTemporary(X, Y, Dir, Size)
		: ColorizeAreaNegativeTemporary(X, Y, Dir, Size);
}

bool AShFieldBase::PlaceShipOnCell(AShCellBase* CellPtr, AShShipBase* ShipPtr)
{
	if (!IsValid(ShipPtr) || !IsValid(CellPtr) || !Field.Contains(CellPtr))
	{
		return false;
	}

	int32 RawID = Field.Find(CellPtr);

	int32 X = INDEX_NONE, Y = INDEX_NONE;
	ConvertID(RawID, X, Y);

	const EShipDirection Dir = ShipPtr->GetShipDirection();
	const EShipSize Size = ShipPtr->GetShipSize();

	if (CheckCanPlaceShip(X, Y, Dir, Size))
	{
		auto Loc = CellPtr->GetActorLocation();
		
		ColorizeAreaPositiveFinally(X, Y, Dir, Size);
		ShipPtr->SetPlacedCellID(X, Y);
		ShipPtr->SetActorLocation({ Loc.X, Loc.Y, Loc.Z + 100. });
		return true;
	}
	else
	{
		return false;
	}
}

void AShFieldBase::RemoveShip(AShShipBase* ShipPtr)
{
	if (!IsValid(ShipPtr))
	{
		return;
	}

	int32 X = INDEX_NONE, Y = INDEX_NONE;

	ShipPtr->GetPlacedCellID(X, Y);
	if (X == INDEX_NONE || Y == INDEX_NONE)
	{
		return;
	}

	SH_VALIDATE(IsValid(FieldConfig), LogMessage::FieldConfigNotValid);

	const int32 Size = static_cast<int32>(ShipPtr->GetShipSize());

	int32 DirX = INDEX_NONE, DirY = INDEX_NONE;
	if (!GetCoeffByDir(ShipPtr->GetShipDirection(), DirX, DirY)) return;

	for (int32 s = 0; s <= Size; ++s)
	{
		auto Cell = GetCell(X + DirX * s, Y + DirY * s);
		Cell->UpdateState(ECellState::EMPTY);
	}

	for (int32 s = 0; s <= Size; ++s)
	{
		for (int32 i = -1; i <= 1; ++i)
		{
			for (int32 j = -1; j <= 1; ++j)
			{
				int32 NextX = X + i + s * DirX;
				int32 NextY = Y + j + s * DirY;

				if (!IsInBounds(NextX, FieldConfig->FieldSizeX) || !IsInBounds(NextY, FieldConfig->FieldSizeY)) continue;
				AShCellBase* Cell = GetCell(NextX, NextY);
				if (!IsShipNearCell(NextX, NextY))
				{
					Cell->UpdateState(ECellState::EMPTY);
				}
			}
		}
	}

	ColorizeAreaPositiveTemporary(X, Y, ShipPtr->GetShipDirection(), ShipPtr->GetShipSize());
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

	SH_VALIDATE_RET(IsValid(World), LogMessage::WorldNotValid, false);
	SH_VALIDATE_RET(IsValid(FieldConfig), LogMessage::FieldConfigNotValid, false);
	SH_VALIDATE_RET(IsValid(CellConfig), LogMessage::CellConfigNotValid, false);
	SH_VALIDATE_RET(IsValid(CellConfig->MeshRef), LogMessage::CellMeshNotValid, false);
	SH_VALIDATE_RET(IsValid(CellConfig->DeadZoneMaterialRef), LogMessage::CellMaterialNotValid, false);
	SH_VALIDATE_RET(IsValid(CellConfig->EmptyMaterialRef), LogMessage::CellMaterialNotValid, false);
	SH_VALIDATE_RET(IsValid(CellConfig->ShippedMaterialRef), LogMessage::CellMaterialNotValid, false);
	SH_VALIDATE_RET(IsValid(CellConfig->ForbiddenMaterialRef), LogMessage::CellMaterialNotValid, false);

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

			CellPtr = World->SpawnActorDeferred<AShCellBase>(AShCellBase::StaticClass(), FTransform(Position));
			CellPtr->SetMaterialSource(ECellState::DEADZONE, CellConfig->DeadZoneMaterialRef);
			CellPtr->SetMaterialSource(ECellState::EMPTY, CellConfig->EmptyMaterialRef);
			CellPtr->SetMaterialSource(ECellState::SHIPPED, CellConfig->ShippedMaterialRef);
			CellPtr->SetMaterialSource(ECellState::FORBIDDEN, CellConfig->ForbiddenMaterialRef);
			CellPtr->FinishSpawning(FTransform(Position));

			checkf(IsValid(CellPtr), TEXT("Failed to spawn cell at [%d][%d]"), i, j);
			
			CellPtr->ApplyMesh(CellConfig->MeshRef);
			CellPtr->ScaleMeshBody(CellConfig->ScaleMesh);

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

bool AShFieldBase::PlaceShipRand(AShShipBase* ShipPtr)
{
	for(int32 Iteration = 0; Iteration < 100000; ++Iteration)
	{
		const int32 X = FMath::RandRange(0, FieldConfig->FieldSizeX - 1);
		const int32 Y = FMath::RandRange(0, FieldConfig->FieldSizeY - 1);
		const EShipDirection Dir = static_cast<EShipDirection>(FMath::RandRange(0, 3));
		ShipPtr->SetShipDirection(Dir);
		if (PlaceShipOnCell(GetCell(X, Y), ShipPtr))
		{
			return true;
		}
	}
	return false;
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
	SH_VALIDATE_RET(IsValid(FieldConfig), LogMessage::FieldConfigNotValid, false);

	int32 DirX = INDEX_NONE, DirY = INDEX_NONE;
	if (!GetCoeffByDir(Direction, DirX, DirY)) return false;

	const int32 Size = static_cast<int32>(ShipSize);
	const int32 EndX = X + Size * DirX;
	const int32 EndY = Y + Size * DirY;

	if (!IsInBounds(EndX, FieldConfig->FieldSizeX) || !IsInBounds(EndY, FieldConfig->FieldSizeY))
		return false;

	for (int32 s = 0; s <= Size; ++s)
	{
		if (GetCell(X + s * DirX, Y + s * DirY)->GetState() != ECellState::EMPTY)
			return false;
	}

	return true;
}

void AShFieldBase::ColorizeAreaPositiveTemporary(int32 X, int32 Y, EShipDirection Direction, EShipSize ShipSize)
{
	SH_VALIDATE(IsValid(FieldConfig), LogMessage::FieldConfigNotValid);

	if (!CheckCanPlaceShip(X, Y, Direction, ShipSize)) return;

	ResetAllCells();

	int32 DirX = INDEX_NONE, DirY = INDEX_NONE;
	if (!GetCoeffByDir(Direction, DirX, DirY)) return;

	const int32 Size = (int32)(ShipSize);
	for (int32 s = 0; s <= Size; ++s)
	{
		for (int32 i = -1; i <= 1; ++i)
		{
			for (int32 j = -1; j <= 1; ++j)
			{
				const int32 NextX = X + i + s * DirX;
				const int32 NextY = Y + j + s * DirY;

				if (!IsInBounds(NextX, FieldConfig->FieldSizeX) || !IsInBounds(NextY, FieldConfig->FieldSizeY)) continue;
				if (AShCellBase * Cell = GetCell(NextX, NextY))
					if (Cell->GetState() == ECellState::EMPTY)	
						Cell->UpdateStateTemporary(ECellState::DEADZONE);
			}
		}
	}
}

void AShFieldBase::ColorizeAreaPositiveFinally(int32 X, int32 Y, EShipDirection Direction, EShipSize ShipSize)
{
	if (!CheckCanPlaceShip(X, Y, Direction, ShipSize)) return;

	ResetAllCells();

	int32 DirX = INDEX_NONE, DirY = INDEX_NONE;
	if (!GetCoeffByDir(Direction, DirX, DirY)) return;

	const int32 Size = static_cast<int32>(ShipSize);
	for (int32 s = 0; s <= Size; ++s)
	{
		int32 NextX = X + s * DirX;
		int32 NextY = Y + s * DirY;

		AShCellBase* Cell = GetCell(NextX, NextY);
		Cell->UpdateState(ECellState::SHIPPED);
		
		for (int32 i = -1; i <= 1; ++i)
		{
			for (int32 j = -1; j <= 1; ++j)
			{
				NextX = X + i + s * DirX;
				NextY = Y + j + s * DirY;

				if (!IsInBounds(NextX, FieldConfig->FieldSizeX) || !IsInBounds(NextY, FieldConfig->FieldSizeY)) continue;
				Cell = GetCell(NextX, NextY);
				if (Cell->GetState() == ECellState::EMPTY)
				{
					Cell->UpdateState(ECellState::DEADZONE);
				}
			}
		}
	}
}

void AShFieldBase::ColorizeAreaNegativeTemporary(int32 X, int32 Y, EShipDirection Direction, EShipSize ShipSize)
{
	SH_VALIDATE(FieldConfig, LogMessage::FieldConfigNotValid);

	ResetAllCells();

	int32 DirX = INDEX_NONE, DirY = INDEX_NONE;
	if (!GetCoeffByDir(Direction, DirX, DirY)) return;

	const int32 Size = (int32)(ShipSize);
	for (int32 s = 0; s <= Size; ++s)
	{
		for (int32 i = -1; i <= 1; ++i)
		{
			for (int32 j = -1; j <= 1; ++j)
			{
				const int32 NextX = X + i + s * DirX;
				const int32 NextY = Y + j + s * DirY;

				if (!IsInBounds(NextX, FieldConfig->FieldSizeX) || !IsInBounds(NextY, FieldConfig->FieldSizeY)) continue;
				if (AShCellBase* Cell = GetCell(NextX, NextY))
				{
					Cell->UpdateStateTemporary(ECellState::FORBIDDEN);
				}
			}
		}
	}
}

void AShFieldBase::ResetAllCells()
{
	SH_VALIDATE(FieldConfig, LogMessage::FieldConfigNotValid);

	for (int32 i = 0; i < FieldConfig->FieldSizeX; ++i)
	{
		for (int32 j = 0; j < FieldConfig->FieldSizeY; ++j)
		{
			if (AShCellBase* Cell = GetCell(i, j)) Cell->ResetStateToActual();
		}
	}
}

bool AShFieldBase::IsShipNearCell(int32 X, int32 Y)
{
	SH_VALIDATE_RET(FieldConfig, LogMessage::FieldConfigNotValid, false);

	for (int32 i = -1; i <= 1; ++i)
	{
		for (int32 j = -1; j <= 1; ++j)
		{
			int32 NextX = X + i;
			int32 NextY = Y + j;

			if (!IsInBounds(NextX, FieldConfig->FieldSizeX) || !IsInBounds(NextY, FieldConfig->FieldSizeY)) continue;
			if (GetCell(NextX, NextY)->GetState() == ECellState::SHIPPED) return true;
		}
	}

	return false;
}

void AShFieldBase::ConvertID(const int32 ID, int32& X, int32& Y)
{
	SH_VALIDATE(IsValid(FieldConfig), LogMessage::FieldConfigNotValid);
	SH_VALIDATE(FieldConfig->FieldSizeY != 0, LogMessage::ZeroDividing);

	X = ID / FieldConfig->FieldSizeY;
	Y = ID % FieldConfig->FieldSizeY;
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

bool AShFieldBase::GetCoeffByDir(EShipDirection Dir, int32& OutCoeffX, int32& OutCoeffY)
{
	switch (Dir)
	{
	case EShipDirection::TOP:
		OutCoeffX = 0;
		OutCoeffY = 1;
		break;
	case EShipDirection::RIGHT:
		OutCoeffX = -1;
		OutCoeffY = 0;
		break;
	case EShipDirection::BOTTOM:
		OutCoeffX = 0;
		OutCoeffY = -1;
		break;
	case EShipDirection::LEFT:
		OutCoeffX = 1;
		OutCoeffY = 0;
		break;
	default:
		return false;
	}

	return true;
}
