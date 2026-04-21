#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FreeformPlayerController.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

UCLASS()
class FREEFORMSEQUENCE_API AFreeformPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	void DPadUp(const FInputActionValue& Value);
	void DPadDown(const FInputActionValue& Value);
	void DPadLeft(const FInputActionValue& Value);
	void DPadRight(const FInputActionValue& Value);
	
private:
	UPROPERTY(EditAnywhere)
	UInputMappingContext* InputMappingFreeform;
	
	UPROPERTY(EditAnywhere)
	UInputAction* IA_DPad_Up;
	
	UPROPERTY(EditAnywhere)
	UInputAction* IA_DPad_Down;
	
	UPROPERTY(EditAnywhere)
	UInputAction* IA_DPad_Left;
	
	UPROPERTY(EditAnywhere)
	UInputAction* IA_DPad_Right;
};