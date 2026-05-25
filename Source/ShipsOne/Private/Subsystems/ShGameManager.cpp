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
	SH_VALIDATE(IsValid(PC), LogMessage_ControllerNotValid);
	SH_VALIDATE(IsValid(PC->GetPawn()), LogMessage_PawnNotValid);
	SH_VALIDATE(PC->Implements<UGameplayControllerInterface>(), LogMessage_ControllerNotImplementsInterface);

	ControllerRef = PC;
	PawnRef = ControllerRef->GetPawn();
}

void UShGameManager::SetupPlayerStateRef(APlayerState* PS)
{
	SH_VALIDATE(!IsValid(PS) && !PS->Implements<UStateUtilityInterface>(), LogMessage_PlayerStateNotValid);

	StateRef = PS;
}

void UShGameManager::RequestShipCreation(const EShipSize ShipSize)
{
	SH_VALIDATE(IsValid(ControllerRef), LogMessage_ControllerNotValid);
	SH_VALIDATE(ControllerRef->Implements<UGameplayControllerInterface>() &&
				ControllerRef->Implements<UGameplayNetworkInterface>(), 
				LogMessage_ControllerNotImplementsInterface);

	IGameplayNetworkInterface::Execute_RequestShipCreation(ControllerRef, ShipSize);
}

void UShGameManager::RemoveCurrentShip()
{
	if (!IsValid(CurrentShip)) return;

	IncreaseShipNum(CurrentShip->GetShipSize());
	RemoveShip(CurrentShip);

	UpdateShipsCollision(true);

	RemoveTickTask(&UShGameManager::SnapShipToCursor);
	RemoveTickTask(&UShGameManager::CheckCellUnderCursor);

	IGameplayControllerInterface::Execute_RemoveShipPlacementInputContext(ControllerRef);
	IGameplayControllerInterface::Execute_ApplyShipRemovementInputContext(ControllerRef);

	OnShipReleased.Broadcast();
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
	SH_VALIDATE(GetWorld(), LogMessage_WorldNotValid);
	SH_VALIDATE(IsValid(ControllerRef), LogMessage_ControllerNotValid);
	SH_VALIDATE(ControllerRef->Implements<UGameplayControllerInterface>(), LogMessage_ControllerNotImplementsInterface);
	SH_VALIDATE(IsValid(PawnRef), LogMessage_PawnNotValid);

	SelfField = GetWorld()->SpawnActor<AShFieldBase>();
	SelfField->SetCellConfig(CellConfig);
	SelfField->SetFieldConfig(FieldConfig);
	SelfField->PlaceInLeft();
	SelfField->BeginInit();

	PawnRef->SetActorLocation(SelfField->GetActorLocation());

	CreateShipPlacingWidget();

	IGameplayControllerInterface::Execute_ApplyCameraActionsInputContext(ControllerRef);
	IGameplayControllerInterface::Execute_ApplyShipRemovementInputContext(ControllerRef);

	AddTickTask(&UShGameManager::TraceUnderCursor);
	AddTickTask(&UShGameManager::CheckShipUnderCursor);
}

void UShGameManager::MakeShip(const EShipSize ShipSize)
{
	SH_VALIDATE(IsValid(GetWorld()), LogMessage_WorldNotValid);
	SH_VALIDATE(CheckCanCreateShip(ShipSize), LogMessage_CannotCreateShip);

	CurrentShip = SpawnShip(ShipSize);

	SH_VALIDATE(IsValid(CurrentShip), LogMessage_ShipNotValid);

	UpdateShipsCollision(false);

	CurrentShip->UpdateOutline(true);

	DecreaseShipNum(ShipSize);
	CreatedShips.Add(CurrentShip);

	IGameplayControllerInterface::Execute_RemoveShipRemovementInputContext(ControllerRef);
	IGameplayControllerInterface::Execute_ApplyShipPlacementInputContext(ControllerRef);

	AddTickTask(&UShGameManager::SnapShipToCursor);
	AddTickTask(&UShGameManager::CheckCellUnderCursor);

	OnShipCaptured.Broadcast();
}

void UShGameManager::PlaceShip()
{
	if (!IsValid(CurrentShip) || !IsValid(SelfField) || !IsValid(CurrentHit.GetActor())) return;

	SH_VALIDATE(IsValid(ControllerRef), LogMessage_ControllerNotValid);
	SH_VALIDATE(ControllerRef->Implements<UGameplayNetworkInterface>(), LogMessage_ControllerNotImplementsInterface);
	SH_VALIDATE(ControllerRef->Implements<UGameplayControllerInterface>(), LogMessage_ControllerNotImplementsInterface);

	auto CurrentCell = Cast<AShCellBase>(CurrentHit.GetActor());
	if (!IsValid(CurrentCell)) return;

	auto Loc = CurrentCell->GetActorLocation();

	if (!SelfField->PlaceShipOnCell(CurrentCell, CurrentShip)) return;

	CurrentShip->SetActorLocation({ Loc.X, Loc.Y, Loc.Z + 100. });
	CurrentShip->UpdateOutline(false);
	RemoveTickTask(&UShGameManager::SnapShipToCursor);
	RemoveTickTask(&UShGameManager::CheckCellUnderCursor);

	CurrentShip = nullptr;

	IGameplayControllerInterface::Execute_RemoveShipPlacementInputContext(ControllerRef);
	IGameplayControllerInterface::Execute_ApplyShipRemovementInputContext(ControllerRef);
		
	if (!Debug_CheckShipsNum())
	{
		IGameplayNetworkInterface::Execute_SendFieldInfo(ControllerRef);
	}

	UpdateShipsCollision(true);

	OnShipReleased.Broadcast();
}

void UShGameManager::PullHoveredShip()
{
	if (!IsValid(HoveredShip))
	{
		return;
	}

	CurrentShip = HoveredShip;
	CurrentShip->UpdateOutline(true);

	UpdateShipsCollision(false);

	SelfField->RemoveShip(CurrentShip);

	IGameplayControllerInterface::Execute_RemoveShipRemovementInputContext(ControllerRef);
	IGameplayControllerInterface::Execute_ApplyShipPlacementInputContext(ControllerRef);

	AddTickTask(&UShGameManager::SnapShipToCursor);
	AddTickTask(&UShGameManager::CheckCellUnderCursor);

	OnShipCaptured.Broadcast();
}

bool UShGameManager::CheckCanCreateShip(EShipSize ShipSize)
{
	SH_VALIDATE_RET(ShipsNum.Contains(ShipSize), LogMessage_ShipSizeNotValid, false);

	return ShipsNum[ShipSize] > 0;
}

AShShipBase* UShGameManager::SpawnShip(const EShipSize ShipSize)
{
	SH_VALIDATE_RET(GetWorld(), LogMessage_WorldNotValid, nullptr);

	AShShipBase* ShipRef = GetWorld()->SpawnActor<AShShipBase>();

	SH_VALIDATE_RET(IsValid(ShipRef), LogMessage_ShipNotValid, nullptr);

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

	CreatedShips.RemoveSwap(ShipToRemove);

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

void UShGameManager::UpdateShipsCollision(bool bEnable)
{
	for (auto Ship : CreatedShips)
	{
		Ship->SetCollision(bEnable);
	}
}

void UShGameManager::TraceUnderCursor(float DeltaSeconds)
{
	SH_VALIDATE(IsValid(ControllerRef), LogMessage_ControllerNotValid);

	ControllerRef->GetHitResultUnderCursor(ECC_GameTraceChannel1, false, CurrentHit);
}

void UShGameManager::SnapShipToCursor(float DeltaSeconds)
{
	SH_VALIDATE(IsValid(ControllerRef), LogMessage_ControllerNotValid);
	SH_VALIDATE(IsValid(ShipConfig), LogMessage_ShipConfigNotValid);
	SH_VALIDATE(IsValid(CurrentShip), LogMessage_ShipNotValid);

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
	if (IsValid(CurrentHit.GetActor()) && CurrentHit.GetActor()->GetClass() == AShCellBase::StaticClass())
	{
		CurrentCell = Cast<AShCellBase>(CurrentHit.GetActor());
	}

	if (CurrentCell != LastCell)
	{
		if (!IsValid(SelfField)) return;
		SelfField->ShipHoverOn(CurrentCell, CurrentShip);
	}

	LastCell = CurrentCell;
}

void UShGameManager::CheckShipUnderCursor(float DeltaSeconds)
{
	if (!IsValid(CurrentHit.GetActor()) ||
		CurrentHit.GetActor()->GetClass() != AShShipBase::StaticClass())
	{
		if (IsValid(HoveredShip))
		{
			HoveredShip->UpdateOutline(false);
		}
		HoveredShip = nullptr;
		return;
	}
	
	if(CurrentHit.GetActor() != HoveredShip)
	{
		if (IsValid(HoveredShip))
		{
			HoveredShip->UpdateOutline(false);
		}
		auto CastedHitShip = Cast<AShShipBase>(CurrentHit.GetActor());
		if (IsValid(CastedHitShip))
		{
			CastedHitShip->UpdateOutline(true);
			HoveredShip = CastedHitShip;
		}
	}
}

void UShGameManager::AddTickTask(FTickTask TaskToAdd)
{
	if (!TickTasks.Contains(TaskToAdd))
	{
		TickTasks.Add(TaskToAdd);
	}
}

void UShGameManager::RemoveTickTask(FTickTask TaskToRemove)
{
	if (TickTasks.Contains(TaskToRemove))
	{
		TickTasks.RemoveSwap(TaskToRemove);
	}
}

void UShGameManager::CreateShipPlacingWidget()
{
	SH_VALIDATE(IsValid(WidgetConfig), LogMessage_WidgetConfigNotValid);
	SH_VALIDATE(IsValid(WidgetConfig->ShipPlacingWidgetClass), LogMessage_ShipPlacementWidgetNotValid);

	ShipPlacingWidgetRef = CreateWidget<UUserWidget>(ControllerRef, WidgetConfig->ShipPlacingWidgetClass);

	SH_VALIDATE(IsValid(ShipPlacingWidgetRef), LogMessage_ShipPlacementWidgetNotValid);

	ShipPlacingWidgetRef->AddToViewport();
}

bool UShGameManager::Debug_CheckShipsNum()
{
	TArray<int32> Values;
	ShipsNum.GenerateValueArray(Values);

	for (auto v : Values)
	{
		if (v != 0) return true;
	}

	return false;
}
