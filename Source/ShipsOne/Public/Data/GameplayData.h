#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(ShLog_Gameplay, All, All)

#define SH_VALIDATE_RET(Cond, Msg, RetVal) \
    if (!(Cond)) { UE_LOG(ShLog_Gameplay, Error, TEXT("%s Func: %hs. Obj: %s."), \
        Msg, __FUNCTION__, *GetName()); return RetVal; }

#define SH_VALIDATE(Cond, Msg) \
    SH_VALIDATE_RET(Cond, Msg, )

namespace LogMessage
{
	// ---- Core / World ---- //
	inline constexpr TCHAR WorldNotValid[]                   = TEXT("World is not valid.");
	inline constexpr TCHAR GamemodeNotValid[]                = TEXT("Gamemode is not valid.");
	inline constexpr TCHAR GameStateNotValid[]               = TEXT("GameState is not valid.");
	inline constexpr TCHAR SubsystemNotValid[]               = TEXT("One of subsystems is not valid.");
	inline constexpr TCHAR EnhancedSubsystemNotValid[]       = TEXT("Enhanced player subsystem is not valid.");
	inline constexpr TCHAR ZeroDividing[]                    = TEXT("Dividing by zero.");
	inline constexpr TCHAR GameManagerInitFailure[]          = TEXT("Game manager initialization failed.");
	inline constexpr TCHAR MoreThan2Detected[]               = TEXT("More than 2 players detected.");

	// ---- Player ---- //
	inline constexpr TCHAR ControllerNotValid[]              = TEXT("Player controller is not valid.");
	inline constexpr TCHAR ControllerNotImplementsInterface[]= TEXT("Player controller does not implement necessary interface.");
	inline constexpr TCHAR ControllerAlreadySet[]            = TEXT("Player controller is already set.");
	inline constexpr TCHAR PlayerStateNotValid[]             = TEXT("Player state is not valid");
	inline constexpr TCHAR PawnNotValid[]                    = TEXT("Player Pawn is not valid.");
	inline constexpr TCHAR PawnNotImplementsInterface[]      = TEXT("Player pawn does not implement interface.");
	inline constexpr TCHAR CursorNotValid[]                  = TEXT("Mouse cursor is not valid.");

	// ---- Input ---- //
	inline constexpr TCHAR EICNotValid[]                     = TEXT("Enhanced input component is not valid.");
	inline constexpr TCHAR InputMappingNotValid[]            = TEXT("Some input mapping context from config is not valid.");
	inline constexpr TCHAR CameraRotationInputNotValid[]     = TEXT("Camera rotation input is not valid.");
	inline constexpr TCHAR CameraMovementInputNotValid[]     = TEXT("Camera movement input is not valid.");
	inline constexpr TCHAR CameraZoomInputNotValid[]         = TEXT("Camera zoom input is not valid.");
	inline constexpr TCHAR ShipPlacementInputNotValid[]      = TEXT("Ship placement input is not valid.");
	inline constexpr TCHAR ShipRotationInputNotValid[]       = TEXT("Ship rotation input is not valid.");

	// ---- Ship ---- //
	inline constexpr TCHAR ShipNotValid[]                    = TEXT("Ship is not valid.");
	inline constexpr TCHAR ShipSizeNotValid[]                = TEXT("Ship size is not valid.");
	inline constexpr TCHAR CannotCreateShip[]                = TEXT("Cannot create a ship.");
	inline constexpr TCHAR ShipBodyMeshNotValid[]            = TEXT("Some ship body mesh is not valid.");
	inline constexpr TCHAR ShipOutlineMaterialNotValid[]     = TEXT("Ship outline material is not valid.");

	// ---- Cell / Field ---- //
	inline constexpr TCHAR CellMeshNotValid[]                = TEXT("Cell mesh is not valid.");
	inline constexpr TCHAR CellMaterialNotValid[]            = TEXT("Some cell material is not valid.");
	inline constexpr TCHAR FindingCellMeshFailure[]          = TEXT("Unable to find cell mesh by path.");

	// ---- Config / Assets ---- //
	inline constexpr TCHAR ShipConfigNotValid[]              = TEXT("Ship config is not valid.");
	inline constexpr TCHAR CellConfigNotValid[]              = TEXT("Cell config is not valid.");
	inline constexpr TCHAR FieldConfigNotValid[]             = TEXT("Field config is not valid.");
	inline constexpr TCHAR WidgetConfigNotValid[]            = TEXT("Widget config is not valid.");
	inline constexpr TCHAR ControllerConfigNotValid[]        = TEXT("Controller config is not valid.");
	inline constexpr TCHAR MeshPtrNotValid[]                 = TEXT("Mesh pointer not valid while applying.");
	inline constexpr TCHAR MaterialPtrNotValid[]             = TEXT("Material pointer not valid.");
	inline constexpr TCHAR BodyMeshNotValid[]                = TEXT("Body mesh is not valid.");

	// ---- UI ---- //
	inline constexpr TCHAR ShipPlacementWidgetNotValid[]     = TEXT("Ship placement widget is not valid.");
}