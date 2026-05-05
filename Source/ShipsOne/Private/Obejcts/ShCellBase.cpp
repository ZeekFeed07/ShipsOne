#include "Obejcts/ShCellBase.h"
#include "Data/GameplayData.h"
#include "Data/Assets/CellDataAsset.h"

AShCellBase::AShCellBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	RootComponent = Mesh;
}

void AShCellBase::BeginPlay()
{
	Super::BeginPlay();
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