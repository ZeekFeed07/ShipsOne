#include "Core/ShMainMenuHUD.h"
#include "Core/ShMainMenuPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Data/UIData.h"

DEFINE_LOG_CATEGORY(ShLog_UI)

AShMainMenuHUD::AShMainMenuHUD()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AShMainMenuHUD::InitMainWidget_Implementation()
{
	if (!IsValid(MainMenuWidgetRef))
	{
		UE_LOG(ShLog_UI, Error, TEXT("Main widget reference is not valid. Func: %s. Obj: %s"), ANSI_TO_TCHAR(__func__), *GetName());
		return;
	}

	MainMenuWidgetRef->RemoveFromParent();
	MainMenuWidgetRef->AddToViewport();
	MainMenuWidgetRef->SetOwningPlayer(GetOwningPawn()->GetController<APlayerController>());
}