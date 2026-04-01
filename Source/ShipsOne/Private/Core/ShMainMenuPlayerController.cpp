#include "Core/ShMainMenuPlayerController.h"
#include "Data/ConnectionData.h"

DEFINE_LOG_CATEGORY(ShLog_Connection);

AShMainMenuPlayerController::AShMainMenuPlayerController()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AShMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SendCheckRequest();
}

void AShMainMenuPlayerController::SendCheckRequest()
{
	FHttpRequestRef NewCheckRequest = FHttpModule::Get().CreateRequest();
	NewCheckRequest->SetURL("http://localhost:8080/ping");
	NewCheckRequest->SetVerb("GET");
	NewCheckRequest->OnProcessRequestComplete().BindUObject(this, &AShMainMenuPlayerController::OnResponseConfirmed);
	NewCheckRequest->ProcessRequest();

	UE_LOG(ShLog_Connection, Display, TEXT("Check request sended. Func: %s. Obj: %s."), ANSI_TO_TCHAR(__func__), *GetName());
}

void AShMainMenuPlayerController::OnResponseConfirmed(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (!bSuccess || !Response.IsValid())
	{
		UE_LOG(ShLog_Connection, Error, TEXT("Failed to reach server! Func: %s. Obj: %s."), ANSI_TO_TCHAR(__func__), *GetName());
		return;
	}

	FString Body = Response->GetContentAsString();
	int32 Code = Response->GetResponseCode();

	UE_LOG(
		ShLog_Connection,
		Display,
		TEXT("Server responded! Code: %d, Body: %s. Func: %s. Obj: %s."),
		Code, *Body, ANSI_TO_TCHAR(__func__), *GetName()
	);
}