#include "Core/ShGameplayPlayerController.h"
#include "Core/ShGameplayPlayerState.h"

#include "Interfaces/ShSubsystemInteraction.h"

#include "Data/GameplayData.h"
#include "Data/Assets/ControllerDataAsset.h"

#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"

DEFINE_LOG_CATEGORY(ShLog_Gameplay);

AShGameplayPlayerController::AShGameplayPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	LoadControllerConfig();
}

void AShGameplayPlayerController::RotateCameraAngleInput(const FInputActionValue& Value)
{	
	OnCameraRotationInput.Broadcast(Value.Get<FVector2D>());
}

void AShGameplayPlayerController::MoveCameraInput(const FInputActionValue& Value)
{
	OnCameraMovementInput.Broadcast(Value.Get<FVector2D>());
}

void AShGameplayPlayerController::ZoomCameraInput(const FInputActionValue& Value)
{
	OnCameraZoomInput.Broadcast(Value.Get<float>());	
}

void AShGameplayPlayerController::PlaceShipInput(const FInputActionValue& Value)
{
	OnPlaceShipInput.Broadcast(Value.Get<bool>());
}

void AShGameplayPlayerController::RotateShipInput(const FInputActionValue& Value)
{
	OnRotateShipInput.Broadcast(Value.Get<float>());
}

void AShGameplayPlayerController::PullShipInput(const FInputActionValue& Value)
{
	OnPullShipInput.Broadcast(Value.Get<bool>());
}

void AShGameplayPlayerController::ApplyCameraInputMapping()
{
	if (!IsLocalController()) return;

	SH_VALIDATE(IsValid(ControllerConfig), LogMessage::ControllerConfigNotValid);
	SH_VALIDATE(IsValid(ControllerConfig->CameraActionsContext), LogMessage::InputMappingNotValid);

	auto EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	SH_VALIDATE(IsValid(EnhancedSubsystem), LogMessage::EnhancedSubsystemNotValid);

	EnhancedSubsystem->AddMappingContext(ControllerConfig->CameraActionsContext, 0);
}

void AShGameplayPlayerController::ApplyShipPlacementInputMapping()
{
	if (!IsLocalController()) return;

	SH_VALIDATE(IsValid(ControllerConfig), LogMessage::ControllerConfigNotValid);
	SH_VALIDATE(IsValid(ControllerConfig->ShipPlacementContext), LogMessage::InputMappingNotValid);

	auto EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	SH_VALIDATE(IsValid(EnhancedSubsystem), LogMessage::EnhancedSubsystemNotValid);

	EnhancedSubsystem->AddMappingContext(ControllerConfig->ShipPlacementContext, 1);
}

void AShGameplayPlayerController::ApplyShipRemovementInputMapping()
{
	if (!IsLocalController()) return;

	SH_VALIDATE(IsValid(ControllerConfig), LogMessage::ControllerConfigNotValid);
	SH_VALIDATE(IsValid(ControllerConfig->ShipRemovementContext), LogMessage::InputMappingNotValid);

	auto EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	SH_VALIDATE(IsValid(EnhancedSubsystem), LogMessage::EnhancedSubsystemNotValid);

	EnhancedSubsystem->AddMappingContext(ControllerConfig->ShipRemovementContext, 1);
}

void AShGameplayPlayerController::RemoveCameraInputMapping()
{
}

void AShGameplayPlayerController::RemoveShipPlacementInputMapping()
{
	if (!IsLocalController()) return;

	SH_VALIDATE(IsValid(ControllerConfig), LogMessage::ControllerConfigNotValid);
	SH_VALIDATE(IsValid(ControllerConfig->ShipPlacementContext), LogMessage::InputMappingNotValid);

	auto EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	SH_VALIDATE(IsValid(EnhancedSubsystem), LogMessage::EnhancedSubsystemNotValid);

	EnhancedSubsystem->RemoveMappingContext(ControllerConfig->ShipPlacementContext);
}

void AShGameplayPlayerController::RemoveShipRemovementInputMapping()
{
	if (!IsLocalController()) return;

	SH_VALIDATE(IsValid(ControllerConfig), LogMessage::ControllerConfigNotValid);
	SH_VALIDATE(IsValid(ControllerConfig->ShipRemovementContext), LogMessage::InputMappingNotValid);

	auto EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	SH_VALIDATE(IsValid(EnhancedSubsystem), LogMessage::EnhancedSubsystemNotValid);

	EnhancedSubsystem->RemoveMappingContext(ControllerConfig->ShipRemovementContext);
}

void AShGameplayPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetupInputMode();
}

void AShGameplayPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	auto EIC = Cast<UEnhancedInputComponent>(InputComponent);

	SH_VALIDATE(IsValid(EIC), LogMessage::EICNotValid);
	SH_VALIDATE(IsValid(ControllerConfig), LogMessage::ControllerConfigNotValid);
	SH_VALIDATE(IsValid(ControllerConfig->CameraRotationInput), LogMessage::CameraRotationInputNotValid);
	SH_VALIDATE(IsValid(ControllerConfig->CameraMovementInput), LogMessage::CameraMovementInputNotValid);
	SH_VALIDATE(IsValid(ControllerConfig->CameraZoomInput), LogMessage::CameraZoomInputNotValid);
	SH_VALIDATE(IsValid(ControllerConfig->ShipPlacementInput), LogMessage::ShipPlacementInputNotValid);
	SH_VALIDATE(IsValid(ControllerConfig->ShipRotationInput), LogMessage::ShipRotationInputNotValid);

	EIC->BindAction(
		ControllerConfig->CameraRotationInput,
		ETriggerEvent::Triggered,
		this,
		&AShGameplayPlayerController::RotateCameraAngleInput);
	EIC->BindAction(
		ControllerConfig->CameraMovementInput,
		ETriggerEvent::Triggered,
		this,
		&AShGameplayPlayerController::MoveCameraInput);
	EIC->BindAction(
		ControllerConfig->CameraZoomInput,
		ETriggerEvent::Triggered,
		this,
		&AShGameplayPlayerController::ZoomCameraInput);
	EIC->BindAction(
		ControllerConfig->ShipPlacementInput,
		ETriggerEvent::Triggered,
		this,
		&AShGameplayPlayerController::PlaceShipInput);
	EIC->BindAction(
		ControllerConfig->ShipRotationInput,
		ETriggerEvent::Triggered,
		this,
		&AShGameplayPlayerController::RotateShipInput);
	EIC->BindAction(
		ControllerConfig->ShipPullInput,
		ETriggerEvent::Triggered,
		this,
		&AShGameplayPlayerController::PullShipInput);
}

void AShGameplayPlayerController::SetupGameManagers()
{
	if (!IsLocalController()) return;
	SH_VALIDATE(GetWorld(), LogMessage::WorldNotValid);

	auto SubsystemsList = GetWorld()->GetSubsystemArrayCopy<UWorldSubsystem>();
	for (auto& Subsystem : SubsystemsList)
	{
		if (Subsystem->Implements<UShSubsystemInteraction>())
		{
			if (auto PlayerStateCasted = Cast<AShGameplayPlayerState>(PlayerState))
			{
				IShSubsystemInteraction::Execute_SetupRefs(Subsystem, this, PlayerStateCasted);
				IShSubsystemInteraction::Execute_StartGame(Subsystem);
			}
		}
	}	
}

void AShGameplayPlayerController::SetupInputMode()
{
	FInputModeGameAndUI InputMode = {};
	InputMode.SetHideCursorDuringCapture(false);

	SetInputMode(InputMode);

	bShowMouseCursor = true;
}

void AShGameplayPlayerController::OnRep_PlayerState()
{
	if (CheckDataReady())
	{
		SetupGameManagers();
	}
}

bool AShGameplayPlayerController::CheckDataReady()
{
	if (!IsValid(PlayerState)) return false;

	return true;
}

void AShGameplayPlayerController::LoadControllerConfig()
{
	ControllerConfig = LoadObject<UControllerDataAsset>(nullptr, *ControllerConfigPath);
}
