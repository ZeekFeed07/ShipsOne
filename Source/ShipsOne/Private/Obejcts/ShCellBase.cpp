#include "Obejcts/ShCellBase.h"
#include "Data/GameplayData.h"
#include "Data/Assets/CellDataAsset.h"

AShCellBase::AShCellBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	RootComponent = Mesh;

	StateMaterialCorrespondence.Add({ECellState::DEADZONE, nullptr});
	StateMaterialCorrespondence.Add({ECellState::EMPTY, nullptr});
	StateMaterialCorrespondence.Add({ECellState::SHIPPED, nullptr});
}

void AShCellBase::BeginPlay()
{
	Super::BeginPlay();

	UpdateState(ECellState::EMPTY);
}

void AShCellBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShCellBase::ApplyMesh(UStaticMesh* MeshBody)
{
	if (!IsValid(MeshBody))
	{
		UE_LOG(
			ShLog_Gameplay,
			Error,
			TEXT("%s Func: %s. Obj: %s"),
			*LogMessage_MeshPtrNotValid,
			TEXT(__FUNCTION__),
			*GetName()
		);
		return;
	}
	Mesh->SetStaticMesh(MeshBody);
}

void AShCellBase::ScaleMeshBody(const FVector& Scale)
{
	Mesh->SetRelativeScale3D(Scale);
}

void AShCellBase::SetMaterialSource(ECellState StateForMaterial, UMaterialInterface* Material) 
{
	if (!IsValid(Material))
	{
		UE_LOG(
			ShLog_Gameplay,
			Error,
			TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_MaterialPtrNotValid,
			TEXT(__FUNCTION__),
			*GetName()
		);
		return;
	}

	StateMaterialCorrespondence.Add({ StateForMaterial, Material });
}

void AShCellBase::SetMeshMaterial(UMaterialInterface* Material)
{
	if (!IsValid(Material))
	{
		UE_LOG(
			ShLog_Gameplay,
			Error,
			TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_MaterialPtrNotValid,
			TEXT(__FUNCTION__),
			*GetName()
		);
		return;
	}
	Mesh->SetMaterial(0, Material);
}

void AShCellBase::UpdateState(ECellState NewState)
{
	if (auto Material = StateMaterialCorrespondence.Find(NewState))
	{
		SetMeshMaterial(*Material);
		CurrentState = NewState;
	}
}

void AShCellBase::UpdateStateTemporary(ECellState TemporaryState)
{
	if (auto Material = StateMaterialCorrespondence.Find(TemporaryState))
	{
		SetMeshMaterial(*Material);
	}
}

void AShCellBase::ResetStateToActual()
{
	if (auto Material = StateMaterialCorrespondence.Find(CurrentState))
	{
		SetMeshMaterial(*Material);
	}
}
