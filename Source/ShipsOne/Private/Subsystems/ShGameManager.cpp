#include "Subsystems/ShGameManager.h"
#include "GameFramework/PlayerState.h"
#include "Obejcts/ShFieldBase.h"
#include "Obejcts/ShShipBase.h"
#include "Interfaces/StateUtilityInterface.h"
#include "Interfaces/GameplayControllerInterface.h"
#include "Blueprint/UserWidget.h"
#include "Data/Assets/WidgetsSettingsDataAsset.h"
#include "Data/Assets/ShipDataAsset.h"

void UShGameManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ShipsNum.Add({ EShipSize::ONE, 4 });
	ShipsNum.Add({ EShipSize::TWO, 3 });
	ShipsNum.Add({ EShipSize::THREE, 2 });
	ShipsNum.Add({ EShipSize::FOUR, 1 });
}

void UShGameManager::Deinitialize()
{
	Super::Deinitialize();
}

void UShGameManager::PostInitialize()
{
	Super::PostInitialize();

	WidgetConfig	= LoadObject<UWidgetsSettingsDataAsset>(nullptr, *WidgetConfigPath);
	ShipConfig		= LoadObject<UShipDataAsset>(nullptr, *ShipConfigPath);
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
	if (!IsValid(PC->GetPawn()))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_PawnNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	ControllerRef = PC;
	PawnRef = ControllerRef->GetPawn();
}

void UShGameManager::SetupPlayerStateRef(APlayerState* PS)
{
	if (!IsValid(PS) || !PS->Implements<UStateUtilityInterface>())
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_PlayerStateNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	StateRef = PS;

	OnCreateFieldAllowed.BindDynamic(this, &UShGameManager::StartPlayerFieldCreation);
	IStateUtilityInterface::Execute_BindToFieldCreationPermission(StateRef, OnCreateFieldAllowed);
}

void UShGameManager::MakeShip(const EShipSize ShipSize)
{
	if (!IsValid(GetWorld()))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_WorldNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	if (!IsValid(ControllerRef))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_ControllerNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	if (!ControllerRef->Implements<UGameplayControllerInterface>())
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_ControllerNotImplementsInterface,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	if (!CheckCanCreateShip(ShipSize))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_CannotCreateShip,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	CurrentShip = SpawnShip(ShipSize);
	if (!IsValid(CurrentShip))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_ShipNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	IGameplayControllerInterface::Execute_ApplyShipPlacementInputContext(ControllerRef);

	GetWorld()->GetTimerManager().SetTimer(
		SnapShipTimer,
		this,
		&UShGameManager::SnapShipToCursor,
		SnapSmoothness,
		true
	);
}

void UShGameManager::RotateShip()
{
	if (!IsValid(CurrentShip)) return;
	CurrentShip->RotateClockwise();
}

void UShGameManager::StartPlayerFieldCreation()
{
	if (!GetWorld())
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_WorldNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	if (!IsValid(StateRef) || !StateRef->Implements<UStateUtilityInterface>())
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_PlayerStateNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	IStateUtilityInterface::Execute_UnbindFromFieldCreationPermission(StateRef, OnCreateFieldAllowed);

	if (!IsValid(ControllerRef))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_ControllerNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	if (!ControllerRef->Implements<UGameplayControllerInterface>())
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_ControllerNotImplementsInterface,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	if (!IsValid(PawnRef))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_PawnNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	SelfField = GetWorld()->SpawnActor<AShFieldBase>();
	SelfField->PlaceInLeft();
	SelfField->BeginInit();

	PawnRef->SetActorLocation(SelfField->GetActorLocation());

	CreateShipPlacingWidget();

	IGameplayControllerInterface::Execute_ApplyCameraActionsInputContext(ControllerRef);
	IGameplayControllerInterface::Execute_ApplyShipPlacementInputContext(ControllerRef);
}

bool UShGameManager::CheckCanCreateShip(EShipSize ShipSize)
{
	if (!ShipsNum.Contains(ShipSize))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_ShipSizeNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return false;
	}
	return ShipsNum[ShipSize] > 0;
}

AShShipBase* UShGameManager::SpawnShip(const EShipSize ShipSize)
{
	if (!GetWorld())
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_WorldNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return nullptr;
	}

	AShShipBase* ShipRef = GetWorld()->SpawnActor<AShShipBase>();
	if (!IsValid(ShipRef))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_ShipNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return nullptr;
	}

	ShipRef->SetShipConfig(ShipConfig);
	ShipRef->SetShipSize(ShipSize);
	ShipRef->SetShipDirection(EShipDirection::TOP);

	return ShipRef;
}

void UShGameManager::SnapShipToCursor()
{
	if (!IsValid(ControllerRef))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_ControllerNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	FHitResult Hit;
	ControllerRef->GetHitResultUnderCursor(ECC_GameTraceChannel1, false, Hit);

	if (Hit.bBlockingHit)
	{
		CurrentShip->SetActorLocation(Hit.GetActor()->GetActorLocation());
	}
}

void UShGameManager::CreateShipPlacingWidget()
{
	if (!IsValid(WidgetConfig))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_WidgetConfigValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	if (!IsValid(WidgetConfig->ShipPlacingWidgetClass))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_ShipPlacementWidgetNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	ShipPlacingWidgetRef = CreateWidget<UUserWidget>(ControllerRef, WidgetConfig->ShipPlacingWidgetClass);
	ShipPlacingWidgetRef->AddToViewport();
}