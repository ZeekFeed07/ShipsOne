#include "Subsystems/ShGeneralManager.h"
#include "Subsystems/ShPlacementManager.h"
#include "Core/ShGameplayPlayerController.h"
#include "Core/ShGameplayPlayerState.h"
#include "Data/GameplayData.h"

DEFINE_LOG_CATEGORY(ShLog_Gameplay);

void UShGeneralManager::SetupRefs_Implementation(AShGameplayPlayerController* CurrentPC, AShGameplayPlayerState* CurrentPS)
{
    ControllerRef = CurrentPC;
    StateRef = CurrentPS;
}

void UShGeneralManager::StartGame_Implementation()
{
	SH_VALIDATE(GetWorld(), LogMessage::WorldNotValid);
    SH_VALIDATE(IsValid(ControllerRef), LogMessage::ControllerNotValid);
    SH_VALIDATE(IsValid(StateRef), LogMessage::PlayerStateNotValid);

    ControllerRef->ApplyCameraInputMapping();

    if(!InitPlacementManager()) return;
}

bool UShGeneralManager::InitPlacementManager()
{
    SH_VALIDATE_RET(GetWorld(), LogMessage::WorldNotValid, false);
    SH_VALIDATE_RET(IsValid(ControllerRef), LogMessage::ControllerNotValid, false);

    PlacementManagerRef = GetWorld()->GetSubsystem<UShPlacementManager>();

    SH_VALIDATE_RET(PlacementManagerRef, LogMessage::SubsystemNotValid, false);

	PlacementManagerRef->SetControllerRef(ControllerRef);
    PlacementManagerRef->SetShouldTick();

    PlacementManagerRef->MakeField();

    return true;
}