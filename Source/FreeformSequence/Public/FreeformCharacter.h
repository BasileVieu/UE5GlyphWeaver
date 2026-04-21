#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "FreeformCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class FREEFORMSEQUENCE_API AFreeformCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFreeformCharacter();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* InPlayerInputComponent) override;
	
protected:
	void DPad(const FInputActionValue& Value);
	void DPadUp(const FInputActionValue& Value);
	void DPadDown(const FInputActionValue& Value);
	void DPadLeft(const FInputActionValue& Value);
	void DPadRight(const FInputActionValue& Value);
	
private:
	UPROPERTY(EditAnywhere)
	UInputMappingContext* InputMappingFreeform;
	
	UPROPERTY(EditAnywhere)
	UInputAction* IA_DPad;
	
	UPROPERTY(EditAnywhere)
	UInputAction* IA_DPad_Up;
	
	UPROPERTY(EditAnywhere)
	UInputAction* IA_DPad_Down;
	
	UPROPERTY(EditAnywhere)
	UInputAction* IA_DPad_Left;
	
	UPROPERTY(EditAnywhere)
	UInputAction* IA_DPad_Right;
};