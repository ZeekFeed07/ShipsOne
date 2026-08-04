#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ShGameplayGamemode.generated.h"

UCLASS()
class SHIPSSERVER_API AShGameplayGamemode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AShGameplayGamemode();

public:
	virtual void PreLogin(const FString& Options, const FString& Address,
		const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage);

	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	bool IsAllPlayersLogged() const;
protected:
	TArray<APlayerController*> LoggedPlayers;
	int32 MaxPlayers = 2;
};
