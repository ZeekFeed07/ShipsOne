#include "Subsystems/ShGameManager.h"
#include "GameFramework/PlayerState.h"
#include "Obejcts/ShFieldBase.h"
#include "Interfaces/StateUtilityInterface.h"

void UShGameManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UShGameManager::Deinitialize()
{
	Super::Deinitialize();
}

void UShGameManager::SetupControllerRef(APlayerController* PC)
{
	if (!IsValid(PC))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_ControllerNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	ControllerRef = PC;
}

void UShGameManager::SetupPlayerStateRef(APlayerState* PS)
{
	if (!IsValid(PS) || !PS->Implements<UStateUtilityInterface>())
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_PlsyerStateNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	StateRef = PS;

	OnCreateFieldAllowed.BindDynamic(this, &UShGameManager::StartPlayerFieldCreation);
	IStateUtilityInterface::Execute_BindToFieldCreationPermission(StateRef, OnCreateFieldAllowed);
	
}

void UShGameManager::StartPlayerFieldCreation()
{
	IStateUtilityInterface::Execute_UnbindFromFieldCreationPermission(StateRef, OnCreateFieldAllowed);

	SelfField = GetWorld()->SpawnActor<AShFieldBase>();
	SelfField->PlaceInLeft();
	SelfField->BeginInit();
}
