#include "Core/ShMainPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

AShMainPlayerState::AShMainPlayerState()
{
}

void AShMainPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(AShMainPlayerState, bAllowedToCreateField, Params);
}

void AShMainPlayerState::AllowToCreateField_Implementation()
{
	if (!HasAuthority()) return;

	bAllowedToCreateField = true;
	MARK_PROPERTY_DIRTY_FROM_NAME(AShMainPlayerState, bAllowedToCreateField, this);

	if (GetNetMode() == NM_Standalone) OnAllowedToCreateField.Broadcast();
}

void AShMainPlayerState::BindToFieldCreationPermission_Implementation(const FOnAllowFieldCreationNotMulticast& Event)
{
	OnAllowedToCreateField.Add(Event);
}

void AShMainPlayerState::UnbindFromFieldCreationPermission_Implementation(const FOnAllowFieldCreationNotMulticast& Event)
{
	OnAllowedToCreateField.Remove(Event);
}

void AShMainPlayerState::OnRep_AllowedToCreateField()
{
	OnAllowedToCreateField.Broadcast();
}
