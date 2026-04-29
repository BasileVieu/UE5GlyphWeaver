#include "GlyphWeaverPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GlyphWeaverSubsystem.h"

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
	
	EnhancedInputComponent->BindAction(IA_Pause, ETriggerEvent::Triggered, this, &AGlyphWeaverPlayerController::SetPause);
	
	EnhancedInputComponent->BindAction(IA_DPad_Up, ETriggerEvent::Triggered, this, &AGlyphWeaverPlayerController::DPadUp);
	EnhancedInputComponent->BindAction(IA_DPad_Down, ETriggerEvent::Triggered, this, &AGlyphWeaverPlayerController::DPadDown);
	EnhancedInputComponent->BindAction(IA_DPad_Left, ETriggerEvent::Triggered, this, &AGlyphWeaverPlayerController::DPadLeft);
	EnhancedInputComponent->BindAction(IA_DPad_Right, ETriggerEvent::Triggered, this, &AGlyphWeaverPlayerController::DPadRight);
}

void AGlyphWeaverPlayerController::SetPause()
{
	IsPaused = !IsPaused;
	
	UE_LOG(LogTemp, Warning, TEXT("Pause %d"), IsPaused);
	
	GetGameInstance()->GetSubsystem<UGlyphWeaverSubsystem>()->SetPause(IsPaused);
}

void AGlyphWeaverPlayerController::DPadUp(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		GetGameInstance()->GetSubsystem<UGlyphWeaverSubsystem>()->AddPlayerGlyphInput(FGlyph("Up", 0));
	}
}

void AGlyphWeaverPlayerController::DPadDown(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		GetGameInstance()->GetSubsystem<UGlyphWeaverSubsystem>()->AddPlayerGlyphInput(FGlyph("Down", 2));
	}
}

void AGlyphWeaverPlayerController::DPadLeft(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		GetGameInstance()->GetSubsystem<UGlyphWeaverSubsystem>()->AddPlayerGlyphInput(FGlyph("Left", 3));
	}
}

void AGlyphWeaverPlayerController::DPadRight(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		GetGameInstance()->GetSubsystem<UGlyphWeaverSubsystem>()->AddPlayerGlyphInput(FGlyph("Right", 1));
	}
}
