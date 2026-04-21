#include "FreeformPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void AFreeformPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		Subsystem->AddMappingContext(InputMappingFreeform, 0);
	}
}

void AFreeformPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	if (!EnhancedInputComponent)
	{
		return;
	}
	
	EnhancedInputComponent->BindAction(IA_DPad_Up, ETriggerEvent::Triggered, this, &AFreeformPlayerController::DPadUp);
	EnhancedInputComponent->BindAction(IA_DPad_Down, ETriggerEvent::Triggered, this, &AFreeformPlayerController::DPadDown);
	EnhancedInputComponent->BindAction(IA_DPad_Left, ETriggerEvent::Triggered, this, &AFreeformPlayerController::DPadLeft);
	EnhancedInputComponent->BindAction(IA_DPad_Right, ETriggerEvent::Triggered, this, &AFreeformPlayerController::DPadRight);
}

void AFreeformPlayerController::DPadUp(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("DPad Up"));
	}
}

void AFreeformPlayerController::DPadDown(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("DPad Down"));
	}
}

void AFreeformPlayerController::DPadLeft(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("DPad Left"));
	}
}

void AFreeformPlayerController::DPadRight(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("DPad Right"));
	}
}