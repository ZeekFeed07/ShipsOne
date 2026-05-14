#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/StateUtilityInterface.h"
#include "Data/GameplayData.h"
#include "ShMainPlayerState.generated.h"

UCLASS()
class SHIPSONE_API AShMainPlayerState :
	public APlayerState,
	public IStateUtilityInterface
{
	GENERATED_BODY()
public:
	AShMainPlayerState();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
