#include "Obejcts/ShFieldBase.h"
#include "Obejcts/ShCellBase.h"
#include "Data/GameplayData.h"

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
	float FieldX = FieldSizeX * CellSize.X / 2.f;
	float FieldY = FieldSizeY * CellSize.Y / 2.f;
	SetActorLocation({ FieldX + CenterGap, FieldY, 0.f });
}

void AShFieldBase::PlaceInRight()
{
	float FieldX = FieldSizeX * CellSize.X / 2.f;
	float FieldY = FieldSizeY * CellSize.Y / 2.f;
	SetActorLocation({ FieldX - CenterGap, FieldY, 0.f }); 
}

void AShFieldBase::BeginInit()
{
	CreateCells();
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

	ClearCells();

	Field.SetNum(FieldSizeX * FieldSizeY);

	FVector Position = FVector::Zero();
	AShCellBase* CellPtr = nullptr;

	float FieldHalfX = FieldSizeX * CellSize.X / 2.f;
	float FieldHalfY = FieldSizeY * CellSize.Y / 2.f;

	float CellHalfX = CellSize.X / 2.f;
	float CellHalfY = CellSize.Y / 2.f;

	for (int32 i = 0; i < FieldSizeX; ++i)
	{
		for (int32 j = 0; j < FieldSizeY; ++j)
		{
			Position = GetActorLocation()
				-FVector(FieldHalfX, FieldHalfY, 0)
				+FVector(CellSize.X * i + CellHalfX, CellHalfY + CellSize.Y * j, 0);
			CellPtr = World->SpawnActor<AShCellBase>(Position, {});
			checkf(IsValid(CellPtr), TEXT("Failed to spawn cell at [%d][%d]"), i, j);
			SetCell(i, j, CellPtr);
		}
	}

	return true;
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

AShCellBase* AShFieldBase::GetCell(int32 X, int32 Y)
{
	checkf(
		(X >= 0 && X < FieldSizeX) && (Y >= 0 && Y < FieldSizeY),
		TEXT("Accessing field out of bounds. X = %d, Y = %d, when XSize = %d and YSize = %d"),
		X, Y, FieldSizeX, FieldSizeY
	);
	return Field[X * FieldSizeY + Y];
}

void AShFieldBase::SetCell(int32 X, int32 Y, AShCellBase* NewItem)
{
	checkf(
		(X >= 0 && X < FieldSizeX) && (Y >= 0 && Y < FieldSizeY),
		TEXT("Accessing field out of bounds. X = %d, Y = %d, when XSize = %d and YSize = %d"),
		X, Y, FieldSizeX, FieldSizeY
	);
	Field[X * FieldSizeY + Y] = NewItem;
}