#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WidgetsSettingsDataAsset.generated.h"

UCLASS()
class SHIPSONE_API UWidgetsSettingsDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ShipPlacingWidgetClass;
};
