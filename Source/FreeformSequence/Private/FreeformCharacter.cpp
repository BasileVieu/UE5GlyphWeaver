#include "FreeformCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

AFreeformCharacter::AFreeformCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
}

void AFreeformCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AFreeformCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFreeformCharacter::SetupPlayerInputComponent(UInputComponent* InPlayerInputComponent)
{
	Super::SetupPlayerInputComponent(InPlayerInputComponent);
}

void AFreeformCharacter::DPad(const FInputActionValue& Value)
{
	FVector2D Vector2D = Value.Get<FVector2D>();
	
	UE_LOG(LogTemp, Warning, TEXT("DPad %f   %f"), Vector2D.X, Vector2D.Y);
}

void AFreeformCharacter::DPadUp(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("DPad Up"));
	
	if (Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("DPad Up"));
	}
}

void AFreeformCharacter::DPadDown(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("DPad Down"));
	}
}

void AFreeformCharacter::DPadLeft(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("DPad Left"));
	}
}

void AFreeformCharacter::DPadRight(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("DPad Right"));
	}
}