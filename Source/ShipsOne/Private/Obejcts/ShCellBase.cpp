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
	SH_VALIDATE(IsValid(MeshBody), LogMessage::MeshPtrNotValid);

	Mesh->SetStaticMesh(MeshBody);
}

void AShCellBase::ScaleMeshBody(const FVector& Scale)
{
	Mesh->SetRelativeScale3D(Scale);
}

void AShCellBase::SetMaterialSource(ECellState StateForMaterial, UMaterialInterface* Material) 
{
	SH_VALIDATE(IsValid(Material), LogMessage::MaterialPtrNotValid);

	StateMaterialCorrespondence.Add({ StateForMaterial, Material });
}

void AShCellBase::SetMeshMaterial(UMaterialInterface* Material)
{
	SH_VALIDATE(IsValid(Material), LogMessage::MaterialPtrNotValid);

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
