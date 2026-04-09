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
	/**
	* @brief Инициалиация главного меню
	*/
	virtual void InitMainWidget_Implementation() override;
	/**
	* @brief Показать главное меню
	*/
	virtual void ShowMainMenuWidget_Implementation() override;
	/**
	* @brief Показать меню комнаты
	*/
	virtual void ShowLobbyWidget_Implementation() override;

protected:
	/**
	* @brief Очистка вьюпорта
	*/
	virtual void ClearViewport();
private:
	UPROPERTY(EditDefaultsOnly, Category = "Main")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> MainMenuWidgetObject;

	UPROPERTY(EditDefaultsOnly, Category = "Main")
	TSubclassOf<UUserWidget> LobbyWidgetClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> LobbyWidgetObject; 

	// ============================== Logging ==============================//
	FString LogMessage_CreatingWidgetFailureMessage = TEXT("Failed to create widget.");
	FString LogMessage_MainMenuWidgeClassFailure	= TEXT("Main menu widget class reference is not valid.");
	FString LogMessage_LobbyWidgeClassFailure		= TEXT("Lobby widget class reference is not valid.");
	// =====================================================================//
};
	