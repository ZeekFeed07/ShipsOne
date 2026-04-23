#include "Obejcts/ShCellBase.h"
#include "Data/GameplayData.h"

AShCellBase::AShCellBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(*CellMeshPath);
	if (!MeshAsset.Succeeded())
	{
		UE_LOG(ShLog_Gameplay,
			Error,
			TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_FindingCellMeshFailure,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	Mesh->SetStaticMesh(MeshAsset.Object);
}

void AShCellBase::BeginPlay()
{
	Super::BeginPlay();	
}

void AShCellBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}