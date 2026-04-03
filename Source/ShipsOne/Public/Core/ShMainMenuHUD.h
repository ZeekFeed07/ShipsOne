#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interfaces/HudUtility.h"
#include "ShMainMenuHUD.generated.h"

UCLASS()
class SHIPSONE_API AShMainMenuHUD : public AHUD, public IHudUtility
{
	GENERATED_BODY()
public:
	AShMainMenuHUD();
public:
	virtual void InitMainWidget_Implementation() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Main")
	TObjectPtr<UUserWidget> MainMenuWidgetRef;
};
	