#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ShGameplayPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class SHIPSONE_API AShGameplayPawn : public APawn
{
	GENERATED_BODY()

public:
	AShGameplayPawn();
public:
	virtual void OnRep_Controller() override;

	UFUNCTION(BlueprintNativeEvent, Category = "GameplayPawn")
	void OnControllerReplicated();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USceneComponent> RootSceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UCameraComponent> CameraComponent;
};
