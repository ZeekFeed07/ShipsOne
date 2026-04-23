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

	// =============================== IStateUtilityInterface Implementation =============================== //

	virtual void AllowToCreateField_Implementation() override;
	virtual void BindToFieldCreationPermission_Implementation(const FOnAllowFieldCreationNotMulticast& Event) override;
	virtual void UnbindFromFieldCreationPermission_Implementation(const FOnAllowFieldCreationNotMulticast& Event) override;

	// ===================================================================================================== //
protected:
	UFUNCTION()
	virtual void OnRep_AllowedToCreateField();

public:
	UPROPERTY()
	FOnAllowFieldCreation OnAllowedToCreateField;

private:

	UPROPERTY(ReplicatedUsing = OnRep_AllowedToCreateField)
	bool bAllowedToCreateField = false;
};
