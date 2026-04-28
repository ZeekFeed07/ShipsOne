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

	if (!IsValid(EIC))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_EICNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	if (!IsValid(ControllerConfig))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_ControllerConfigNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	if (!IsValid(ControllerConfig->CameraRotationInput))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_CameraRotationInputNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	if (!IsValid(ControllerConfig->CameraMovementInput))
	if (!IsValid(ControllerConfig->CameraZoomInput))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_CameraZoomInputNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	if (!IsValid(ControllerConfig->ShipPlacementInput))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_ShipPlacementInputNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	if (!IsValid(ControllerConfig->ShipRotationInput))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_ShipRotationInputNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

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
}

void AShGameplayPlayerController::InitManager()
{
	if (!GetWorld())
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_WorldNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	Manager = GetWorld()->GetSubsystem<UShGameManager>();
	if(!IsValid(Manager))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_SubsystemNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	APlayerState* State = GetPlayerState<APlayerState>();
	if (!IsValid(State))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_PlayerStateNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

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
	if (!IsValid(GetPawn()))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_PawnNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	if (!GetPawn()->Implements<UGameplayPawnInterface>())
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_PawnNotImplementsInterface,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	FVector2D Angle = Value.Get<FVector2D>();
	IGameplayPawnInterface::Execute_RotateCameraAngle2D(GetPawn(), Angle);
}

void AShGameplayPlayerController::MoveCameraInput(const FInputActionValue& Value)
{
	if (!IsValid(GetPawn()))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_PawnNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	if (!GetPawn()->Implements<UGameplayPawnInterface>())
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_PawnNotImplementsInterface,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	FVector2D Delta = Value.Get<FVector2D>();
	IGameplayPawnInterface::Execute_MoveCamera2D(GetPawn(), Delta);
}

void AShGameplayPlayerController::ZoomCameraInput(const FInputActionValue& Value)
{
	if (!IsValid(GetPawn()))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_PawnNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	if (!GetPawn()->Implements<UGameplayPawnInterface>())
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_PawnNotImplementsInterface,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	float ZoomValue = Value.Get<float>();
	IGameplayPawnInterface::Execute_ZoomCamera(GetPawn(), ZoomValue);
}

void AShGameplayPlayerController::PlaceShipInput(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, FString("Triggered"));
}

void AShGameplayPlayerController::RotateShipInput(const FInputActionValue& Value)
{
	if (!IsValid(Manager))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_SubsystemNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	Manager->RotateShip();
}

void AShGameplayPlayerController::SetupInputMode()
{
	FInputModeGameAndUI InputMode = {};
	InputMode.SetHideCursorDuringCapture(false);

	SetInputMode(InputMode);

	bShowMouseCursor = true;
}

void AShGameplayPlayerController::ApplyCameraActionsInputContext_Implementation()
{
	if (!IsLocalController()) return;
	if (!IsValid(ControllerConfig))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_ControllerConfigNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	if (!IsValid(ControllerConfig->CameraActionsContext))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_InputMappingNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!IsValid(EnhancedSubsystem))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_EnhancedSubsystemNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	EnhancedSubsystem->AddMappingContext(ControllerConfig->CameraActionsContext, 0);
}

void AShGameplayPlayerController::ApplyShipPlacementInputContext_Implementation()
{
	if (!IsLocalController()) return;
	if (!IsValid(ControllerConfig))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_ControllerConfigNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	if (!IsValid(ControllerConfig->ShipPlacementContext))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_InputMappingNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!IsValid(EnhancedSubsystem))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_EnhancedSubsystemNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	EnhancedSubsystem->AddMappingContext(ControllerConfig->ShipPlacementContext, 1);
}

void AShGameplayPlayerController::RemoveShipPlacementInputContext_Implementation()
{
	if (!IsLocalController()) return;
	if (!IsValid(ControllerConfig))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_ControllerConfigNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}
	if (!IsValid(ControllerConfig->ShipPlacementContext))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_InputMappingNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!IsValid(EnhancedSubsystem))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_EnhancedSubsystemNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	EnhancedSubsystem->RemoveMappingContext(ControllerConfig->ShipPlacementContext);
}

void AShGameplayPlayerController::ServerControllerReadyRPC_Implementation()
{
	if (!GetWorld())
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_WorldNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	AShGameplayGameMode* GM = GetWorld()->GetAuthGameMode<AShGameplayGameMode>();
	if (!IsValid(GM))
	{
		UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %s. Obj: %s."),
			*LogMessage_GamemodeNotValid,
			TEXT(__FUNCTION__),
			*GetName());
		return;
	}

	GM->PlayerReady(this);
}
