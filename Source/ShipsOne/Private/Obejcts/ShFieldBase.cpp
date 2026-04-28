#include "Obejcts/ShFieldBase.h"
#include "Obejcts/ShCellBase.h"
#include "Data/GameplayData.h"
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
	float FieldX = FieldConfig->FieldSizeX * CellConfig->CellSize.X / 2.f;
	float FieldY = FieldConfig->FieldSizeY * CellConfig->CellSize.Y / 2.f;
	SetActorLocation({ FieldX + FieldConfig->CenterGap, FieldY, 0.f });
}

void AShFieldBase::PlaceInRight()
{
	float FieldX = FieldConfig->FieldSizeX * CellConfig->CellSize.X / 2.f;
	float FieldY = FieldConfig->FieldSizeY * CellConfig->CellSize.Y / 2.f;
	SetActorLocation({ FieldX - FieldConfig->CenterGap, FieldY, 0.f });
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
	ApplyFieldConfig();
	ApplyCellConfig();
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

void AShFieldBase::ApplyFieldConfig()
{
	UFieldDataAsset* ProcessingConfig = LoadObject<UFieldDataAsset>(nullptr, *FieldConfigPath);
	if (!IsValid(ProcessingConfig))
	{
		UE_LOG(
			ShLog_Gameplay,
			Error,
			TEXT("%s Func: %s. Obj: %s"),
			*LogMessage_FieldConfigNotApplied,
			TEXT(__FUNCTION__),
			*GetName()
		);
		return;
	}

	FieldConfig = ProcessingConfig;
}

void AShFieldBase::ApplyCellConfig()
{
	UCellDataAsset* ProcessingConfig = LoadObject<UCellDataAsset>(nullptr, *CellConfigPath);
	if (!IsValid(ProcessingConfig))
	{
		UE_LOG(
			ShLog_Gameplay,
			Error,
			TEXT("%s Func: %s. Obj: %s"),
			*LogMessage_CellConfigNotApplied,
			TEXT(__FUNCTION__),
			*GetName()
		);
		return;
	}

	CellConfig = ProcessingConfig;
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