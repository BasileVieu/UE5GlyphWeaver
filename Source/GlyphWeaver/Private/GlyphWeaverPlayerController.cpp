#include "GlyphWeaverPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void AGlyphWeaverPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		Subsystem->AddMappingContext(InputMappingFreeform, 0);
	}
}

void AGlyphWeaverPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	if (!EnhancedInputComponent)
	{
		return;
	}
	
	EnhancedInputComponent->BindAction(IA_DPad_Up, ETriggerEvent::Triggered, this, &AGlyphWeaverPlayerController::DPadUp);
	EnhancedInputComponent->BindAction(IA_DPad_Down, ETriggerEvent::Triggered, this, &AGlyphWeaverPlayerController::DPadDown);
	EnhancedInputComponent->BindAction(IA_DPad_Left, ETriggerEvent::Triggered, this, &AGlyphWeaverPlayerController::DPadLeft);
	EnhancedInputComponent->BindAction(IA_DPad_Right, ETriggerEvent::Triggered, this, &AGlyphWeaverPlayerController::DPadRight);
}

void AGlyphWeaverPlayerController::DPadUp(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("DPad Up"));
	}
}

void AGlyphWeaverPlayerController::DPadDown(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("DPad Down"));
	}
}

void AGlyphWeaverPlayerController::DPadLeft(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("DPad Left"));
	}
}

void AGlyphWeaverPlayerController::DPadRight(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("DPad Right"));
	}
}