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
	IHudUtility::Execute_ShowMainMenuWidget(this);
}

void AShMainMenuHUD::ShowMainMenuWidget_Implementation()
{
	if (!IsValid(MainMenuWidgetClass))
	{
		UE_LOG(ShLog_UI, Error, TEXT("%s Func: %s. Obj: %s."), *LogMessage_MainMenuWidgeClassFailure, TEXT(__FUNCTION__), *GetName());
		return;
	}

	ClearViewport();

	MainMenuWidgetObject = CreateWidget(GetOwningPlayerController(), MainMenuWidgetClass);

	if (!IsValid(MainMenuWidgetObject))
	{
		UE_LOG(ShLog_UI, Error, TEXT("%s Func: %s. Obj: %s."), *LogMessage_CreatingWidgetFailureMessage, TEXT(__FUNCTION__), *GetName());
		return;
	}

	MainMenuWidgetObject->AddToViewport();
}

void AShMainMenuHUD::ShowLobbyWidget_Implementation()
{
	if (!IsValid(LobbyWidgetClass))
	{
		UE_LOG(ShLog_UI, Error, TEXT("%s Func: %s. Obj: %s."), *LogMessage_LobbyWidgeClassFailure, TEXT(__FUNCTION__), *GetName());
		return;
	}

	ClearViewport();

	LobbyWidgetObject = CreateWidget(GetOwningPlayerController(), LobbyWidgetClass);

	if (!IsValid(LobbyWidgetObject))
	{
		UE_LOG(ShLog_UI, Error, TEXT("%s Func: %s. Obj: %s."), *LogMessage_CreatingWidgetFailureMessage, TEXT(__FUNCTION__), *GetName());
		return;
	}

	LobbyWidgetObject->AddToViewport();
}

void AShMainMenuHUD::ClearViewport()
{
	if (IsValid(MainMenuWidgetObject) && MainMenuWidgetObject->IsInViewport())
	{
		MainMenuWidgetObject->RemoveFromParent();
		MainMenuWidgetObject = nullptr;
	}

	if (IsValid(LobbyWidgetObject) && LobbyWidgetObject->IsInViewport())
	{
		LobbyWidgetObject->RemoveFromParent();
		LobbyWidgetObject = nullptr;
	}
}
