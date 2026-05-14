#include "Subsystems/ShGameManager.h"
#include "GameFramework/PlayerState.h"
#include "Obejcts/ShFieldBase.h"
#include "Obejcts/ShShipBase.h"
#include "Obejcts/ShCellBase.h"
#include "Interfaces/StateUtilityInterface.h"
#include "Interfaces/GameplayControllerInterface.h"
#include "Interfaces/GameplayNetworkInterface.h"
#include "Blueprint/UserWidget.h"
#include "Data/Assets/WidgetsSettingsDataAsset.h"
#include "Data/Assets/ShipDataAsset.h"
#include "Data/Assets/FieldDataAsset.h"
#include "Tickable.h"
#include "Data/Assets/CellDataAsset.h"

void UShGameManager::Tick(float DeltaTime)
{
	for(auto& Task : TickTasks)
	{
		(this->*Task)(DeltaTime);
	}
}

TStatId UShGameManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UShGameManager, STATGROUP_Tickables);
}

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

	ShipConfig		= LoadObject<UShipDataAsset>(nullptr, *ShipConfigPath);
	FieldConfig		= LoadObject<UFieldDataAsset>(nullptr, *FieldConfigPath);
	CellConfig		= LoadObject<UCellDataAsset>(nullptr, *CellConfigPath);
	WidgetConfig	= LoadObject<UWidgetsSettingsDataAsset>(nullptr, *WidgetConfigPath);
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
	if (!PC->Implements<UGameplayControllerInterface>())
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_ControllerNotImplementsInterface,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	ControllerRef = PC;
	PawnRef = ControllerRef->GetPawn();

	OnAllowFieldCreation.BindDynamic(this, &UShGameManager::StartPlayerFieldCreation);
	OnAllowShipCreation.BindDynamic(this, &UShGameManager::OnShipCreationApproved);

	IGameplayControllerInterface::Execute_BindToFieldCreation(ControllerRef, OnAllowFieldCreation);
	IGameplayControllerInterface::Execute_BindToShipCreation(ControllerRef, OnAllowShipCreation);
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
}

void UShGameManager::MakeShip(const EShipSize ShipSize)
{
	if (!IsValid(ControllerRef))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_ControllerNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	if (!ControllerRef->Implements<UGameplayControllerInterface>() || !ControllerRef->Implements<UGameplayNetworkInterface>())
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_ControllerNotImplementsInterface,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	IGameplayNetworkInterface::Execute_RequestShipCreation(ControllerRef, ShipSize);
}

void UShGameManager::RemoveCurrentShip()
{
	if (!IsValid(CurrentShip)) return;

	IncreaseShipNum(CurrentShip->GetShipSize());
	RemoveShip(CurrentShip);
}

void UShGameManager::RotateShipClockwise()
{
	if (!IsValid(CurrentShip)) return;
	LastCell = nullptr;
	CurrentShip->RotateClockwise();
}

void UShGameManager::RotateShipCounterClockwise()
{
	if (!IsValid(CurrentShip)) return;
	LastCell = nullptr;
	CurrentShip->RotateCounterClockwise();
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

	IGameplayControllerInterface::Execute_UnbindFromFieldCreation(ControllerRef, OnAllowFieldCreation);

	SelfField = GetWorld()->SpawnActor<AShFieldBase>();
	SelfField->SetCellConfig(CellConfig);
	SelfField->SetFieldConfig(FieldConfig);
	SelfField->PlaceInLeft();
	SelfField->BeginInit();

	PawnRef->SetActorLocation(SelfField->GetActorLocation());

	CreateShipPlacingWidget();

	IGameplayControllerInterface::Execute_ApplyCameraActionsInputContext(ControllerRef);
	IGameplayControllerInterface::Execute_ApplyShipPlacementInputContext(ControllerRef);
}

void UShGameManager::OnShipCreationApproved(const EShipSize ShipSize)
{
	if (!IsValid(GetWorld()))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_WorldNotValid,
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
		CurrentShip = nullptr;
		return;
	}

	DecreaseShipNum(ShipSize);
	CreatedShips.Add(CurrentShip);

	IGameplayControllerInterface::Execute_ApplyShipPlacementInputContext(ControllerRef);

	TickTasks.Add(&UShGameManager::TraceUnderCursor);
	TickTasks.Add(&UShGameManager::SnapShipToCursor);
	TickTasks.Add(&UShGameManager::CheckCellUnderCursor);
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
	ShipRef->SetCellConfig(CellConfig);
	ShipRef->SetShipSize(ShipSize);
	ShipRef->SetShipDirection(EShipDirection::TOP);
	ShipRef->NormalizeForDirection();

	return ShipRef;
}

void UShGameManager::RemoveShip(AShShipBase*& ShipToRemove)
{
	if (!IsValid(ShipToRemove)) return;

	ShipToRemove->Destroy();
	ShipToRemove = nullptr;
}

void UShGameManager::IncreaseShipNum(EShipSize ShipSize)
{
	int32* NumbersOfShip = ShipsNum.Find(ShipSize);
	if (NumbersOfShip)
	{
		(*NumbersOfShip)++;
		OnShipsNumStateChange.Broadcast(ShipSize, *NumbersOfShip);
	}
}

void UShGameManager::DecreaseShipNum(EShipSize ShipSize)
{
	int32* NumbersOfShip = ShipsNum.Find(ShipSize);
	if (NumbersOfShip)
	{
		(*NumbersOfShip)--;
		OnShipsNumStateChange.Broadcast(ShipSize, *NumbersOfShip);
	}
}

void UShGameManager::TraceUnderCursor(float DeltaSeconds)
{
	if (!IsValid(ControllerRef))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_ControllerNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	ControllerRef->GetHitResultUnderCursor(ECC_GameTraceChannel1, false, CurrentHit);
}

void UShGameManager::SnapShipToCursor(float DeltaSeconds)
{
	if (!IsValid(ControllerRef))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_ControllerNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	if (!IsValid(ShipConfig))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_ShipConfigNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	if (!IsValid(CurrentShip))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_ShipNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	FVector SnapTarget = FVector::ZeroVector;
	if (IsValid(CurrentHit.GetActor()) && CurrentHit.GetActor()->GetClass() == AShCellBase::StaticClass() && CurrentHit.bBlockingHit)
	{
		SnapTarget = CurrentHit.GetActor()->GetActorLocation()
			+ FVector(0.f, 0.f, ShipConfig->PlacingVerticalOffset);
	}
	else
	{
		FVector WorldLocation, WorldDirection;
		ControllerRef->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
		SnapTarget = WorldLocation + WorldDirection * ShipConfig->PlacingDistanceFromCamera;
	}

	const FVector Result = FMath::VInterpTo(
		CurrentShip->GetActorLocation(),
		SnapTarget,
		GetWorld()->GetDeltaSeconds(),
		ShipConfig->SnapToCursorInterpolationSpeed
	);

	CurrentShip->SetActorLocation(Result);
}

void UShGameManager::CheckCellUnderCursor(float DeltaSeconds)
{
	AShCellBase* CurrentCell = nullptr;
	if (IsValid(CurrentHit.GetActor()))
	{
		CurrentCell = Cast<AShCellBase>(CurrentHit.GetActor());
		if (!IsValid(CurrentCell)) return;
	}

	if (CurrentCell != LastCell)
	{
		SelfField->ShipHoverOn(CurrentCell, CurrentShip);
	}

	LastCell = CurrentCell;
}

void UShGameManager::CreateShipPlacingWidget()
{
	if (!IsValid(WidgetConfig))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_WidgetConfigNotValid,
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
	
	if(!IsValid(ShipPlacingWidgetRef))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s. Func: %s. Obj: %s."),
			*LogMessage_ShipPlacementWidgetNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	ShipPlacingWidgetRef->AddToViewport();
}