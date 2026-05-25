#include "Core/ShGameplayPlayerController.h"
#include "Core/ShGameplayGameMode.h"
#include "GameFramework/PlayerState.h"
#include "Data/GameplayData.h"
#include "Data/Assets/ControllerDataAsset.h"
#include "Subsystems/ShGameManager.h"
#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Interfaces/GameplayPawnInterface.h"

AShGameplayPlayerController::AShGameplayPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	LoadControllerConfig();
}

void AShGameplayPlayerController::RequestShipCreation_Implementation(EShipSize ShipSize)
{
	ServerRequestShipCreation(ShipSize);
}

void AShGameplayPlayerController::SendFieldInfo_Implementation()
{
	ServerSendFieldInfo();
}

void AShGameplayPlayerController::ApproveFieldCreation_Implementation()
{
	ClientCreateFieldApproved();
}

void AShGameplayPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitManager();
	SetupInputMode();
}

void AShGameplayPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

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

void AShGameplayPlayerController::InitManager()
{
	SH_VALIDATE(GetWorld(), LogMessage::WorldNotValid);

	Manager = GetWorld()->GetSubsystem<UShGameManager>();
	SH_VALIDATE(IsValid(Manager), LogMessage::SubsystemNotValid);

	APlayerState* State = GetPlayerState<APlayerState>();
	SH_VALIDATE(IsValid(State), LogMessage::PlayerStateNotValid);

	Manager->SetupControllerRef(this);
	Manager->SetupPlayerStateRef(State);

	ServerControllerReadyRPC();
}

void AShGameplayPlayerController::LoadControllerConfig()
{
	ControllerConfig = LoadObject<UControllerDataAsset>(nullptr, *ControllerConfigPath);
}

void AShGameplayPlayerController::RotateCameraAngleInput(const FInputActionValue& Value)
{
	SH_VALIDATE(IsValid(GetPawn()), LogMessage::PawnNotValid);
	SH_VALIDATE(GetPawn()->Implements<UGameplayPawnInterface>(), LogMessage::PawnNotImplementsInterface);

	FVector2D Angle = Value.Get<FVector2D>();
	IGameplayPawnInterface::Execute_RotateCameraAngle2D(GetPawn(), Angle);
}

void AShGameplayPlayerController::MoveCameraInput(const FInputActionValue& Value)
{
	SH_VALIDATE(IsValid(GetPawn()), LogMessage::PawnNotValid);
	SH_VALIDATE(GetPawn()->Implements<UGameplayPawnInterface>(), LogMessage::PawnNotImplementsInterface);

	FVector2D Delta = Value.Get<FVector2D>();
	IGameplayPawnInterface::Execute_MoveCamera2D(GetPawn(), Delta);
}

void AShGameplayPlayerController::ZoomCameraInput(const FInputActionValue& Value)
{
	SH_VALIDATE(IsValid(GetPawn()), LogMessage::PawnNotValid);
	SH_VALIDATE(GetPawn()->Implements<UGameplayPawnInterface>(), LogMessage::PawnNotImplementsInterface);

	float ZoomValue = Value.Get<float>();
	IGameplayPawnInterface::Execute_ZoomCamera(GetPawn(), ZoomValue);
}

void AShGameplayPlayerController::PlaceShipInput(const FInputActionValue& Value)
{
	Manager->PlaceShip();
}

void AShGameplayPlayerController::RotateShipInput(const FInputActionValue& Value)
{
	SH_VALIDATE(IsValid(Manager), LogMessage::SubsystemNotValid);

	float RotationValue = Value.Get<float>();

	RotationValue < 0 ? Manager->RotateShipClockwise() : Manager->RotateShipCounterClockwise();
}

void AShGameplayPlayerController::PullShipInput(const FInputActionValue& Value)
{
	Manager->PullHoveredShip();
}

void AShGameplayPlayerController::SetupInputMode()
{
	FInputModeGameAndUI InputMode = {};
	InputMode.SetHideCursorDuringCapture(false);

	SetInputMode(InputMode);

	bShowMouseCursor = true;
}

void AShGameplayPlayerController::ServerRequestShipCreation_Implementation(const EShipSize ShipSize)
{
	// todo: потом надо сделать валидацию

	ClientCreateShipApproved(ShipSize);
}

void AShGameplayPlayerController::ServerSendFieldInfo_Implementation()
{
	// todo: потом надо сделать валидацию

	ClientPlayerCanStart();
}

void AShGameplayPlayerController::ClientCreateFieldApproved_Implementation()
{
	Manager->StartPlayerFieldCreation();
}

void AShGameplayPlayerController::ClientCreateShipApproved_Implementation(const EShipSize ShipSize)
{
	Manager->MakeShip(ShipSize);
}

void AShGameplayPlayerController::ClientPlayerCanStart_Implementation()
{
	Manager->OnGameReady.Broadcast(true);
}

void AShGameplayPlayerController::ApplyCameraActionsInputContext_Implementation()
{
	if (!IsLocalController()) return;

	SH_VALIDATE(IsValid(ControllerConfig), LogMessage::ControllerConfigNotValid);
	SH_VALIDATE(IsValid(ControllerConfig->CameraActionsContext), LogMessage::InputMappingNotValid);

	UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	SH_VALIDATE(IsValid(EnhancedSubsystem), LogMessage::EnhancedSubsystemNotValid);

	EnhancedSubsystem->AddMappingContext(ControllerConfig->CameraActionsContext, 0);
}

void AShGameplayPlayerController::ApplyShipPlacementInputContext_Implementation()
{
	if (!IsLocalController()) return;

	SH_VALIDATE(IsValid(ControllerConfig), LogMessage::ControllerConfigNotValid);
	SH_VALIDATE(IsValid(ControllerConfig->ShipPlacementContext), LogMessage::InputMappingNotValid);

	UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	SH_VALIDATE(IsValid(EnhancedSubsystem), LogMessage::EnhancedSubsystemNotValid);

	EnhancedSubsystem->AddMappingContext(ControllerConfig->ShipPlacementContext, 1);
}

void AShGameplayPlayerController::ApplyShipRemovementInputContext_Implementation()
{
	if (!IsLocalController()) return;

	SH_VALIDATE(IsValid(ControllerConfig), LogMessage::ControllerConfigNotValid);
	SH_VALIDATE(IsValid(ControllerConfig->ShipRemovementContext), LogMessage::InputMappingNotValid);

	UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	SH_VALIDATE(IsValid(EnhancedSubsystem), LogMessage::EnhancedSubsystemNotValid);

	EnhancedSubsystem->AddMappingContext(ControllerConfig->ShipRemovementContext, 1);
}

void AShGameplayPlayerController::RemoveShipPlacementInputContext_Implementation()
{
	if (!IsLocalController()) return;

	SH_VALIDATE(IsValid(ControllerConfig), LogMessage::ControllerConfigNotValid);
	SH_VALIDATE(IsValid(ControllerConfig->ShipPlacementContext), LogMessage::InputMappingNotValid);

	UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	SH_VALIDATE(IsValid(EnhancedSubsystem), LogMessage::EnhancedSubsystemNotValid);

	EnhancedSubsystem->RemoveMappingContext(ControllerConfig->ShipPlacementContext);
}

void AShGameplayPlayerController::RemoveShipRemovementInputContext_Implementation()
{
	if (!IsLocalController()) return;

	SH_VALIDATE(IsValid(ControllerConfig), LogMessage::ControllerConfigNotValid);
	SH_VALIDATE(IsValid(ControllerConfig->ShipRemovementContext), LogMessage::InputMappingNotValid);

	UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	SH_VALIDATE(IsValid(EnhancedSubsystem), LogMessage::EnhancedSubsystemNotValid);

	EnhancedSubsystem->RemoveMappingContext(ControllerConfig->ShipRemovementContext);
}

void AShGameplayPlayerController::ServerControllerReadyRPC_Implementation()
{
	SH_VALIDATE(GetWorld(), LogMessage::WorldNotValid);

	AShGameplayGameMode* GM = GetWorld()->GetAuthGameMode<AShGameplayGameMode>();
	SH_VALIDATE(IsValid(GM), LogMessage::GamemodeNotValid);

	GM->PlayerReady(this);
}
