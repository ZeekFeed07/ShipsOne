#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/ConnectionData.h"
#include "ShLobbyMenu.generated.h"

UCLASS()
class SHIPSONE_API UShLobbyMenu : public UUserWidget
{
	GENERATED_BODY()
	// TODO: удалить, метод не используется
	UFUNCTION(BlueprintCallable, Category = "Info")
	virtual void SetCreatedRoom(const FRoomInfo& NewRoom);

private:
	// TODO: удалить, переменная не используется
	FRoomInfo CreatedRoom;
}; 
