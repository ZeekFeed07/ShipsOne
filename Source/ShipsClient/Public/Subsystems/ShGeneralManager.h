#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Interfaces/ShSubsystemInteraction.h"
#include "ShGeneralManager.generated.h"

class UShPlacementManager;
class AShGameplayPlayerController;
class AShGameplayPlayerState;

UCLASS()
class SHIPSCLIENT_API UShGeneralManager :
    public UWorldSubsystem,
    public IShSubsystemInteraction
{
    GENERATED_BODY()

public:
    // =============================== IShSubsystemInteraction =============================== //
    virtual void SetupRefs_Implementation(AShGameplayPlayerController* CurrentPC, AShGameplayPlayerState* CurrentPS) override;
    virtual void StartGame_Implementation() override;
    // ======================================================================================= //
private:
	virtual bool InitPlacementManager();
private:
    TObjectPtr<AShGameplayPlayerController> ControllerRef = nullptr;
    TObjectPtr<APlayerState> StateRef = nullptr;
    TObjectPtr<UShPlacementManager> PlacementManagerRef = nullptr;    
};
