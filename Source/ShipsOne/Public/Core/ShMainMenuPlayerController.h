#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "ShMainMenuPlayerController.generated.h"

/**
 * @brief 
 */
UCLASS()
class SHIPSONE_API AShMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AShMainMenuPlayerController();
public:
	UFUNCTION(BlueprintCallable, Category = "Quit")
	virtual void RequestQuitGame();
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputMode();

private:
	void SendCheckRequest();
	void OnResponseConfirmed(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);
};
