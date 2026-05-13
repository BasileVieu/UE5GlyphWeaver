#include "GlyphWeaverPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GlyphPuzzle/GlyphDataAsset.h"
#include "GlyphWeaverSubsystem.h"

void AGlyphWeaverPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		Subsystem->AddMappingContext(InputMappingFreeform, 0);
	}
	
	UGlyphPuzzleDataAsset* PuzzleDataAsset = GlyphPuzzleDataAsset.LoadSynchronous();	
	GetGameInstance()->GetSubsystem<UGlyphWeaverSubsystem>()->SetupPuzzle(PuzzleDataAsset);
	
	UGlyphDataAsset* UpDataAsset = PuzzleGlyphDataAssetUp.LoadSynchronous();
	GlyphUp = UpDataAsset->CreateGlyph();
	
	UGlyphDataAsset* DownDataAsset = PuzzleGlyphDataAssetDown.LoadSynchronous();
	GlyphDown = DownDataAsset->CreateGlyph();
	
	UGlyphDataAsset* LeftDataAsset = PuzzleGlyphDataAssetLeft.LoadSynchronous();
	GlyphLeft = LeftDataAsset->CreateGlyph();
	
	UGlyphDataAsset* RightDataAsset = PuzzleGlyphDataAssetRight.LoadSynchronous();
	GlyphRight = RightDataAsset->CreateGlyph();
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
		GetGameInstance()->GetSubsystem<UGlyphWeaverSubsystem>()->AddPlayerGlyphInput(GlyphUp);
	}
}

void AGlyphWeaverPlayerController::DPadDown(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		GetGameInstance()->GetSubsystem<UGlyphWeaverSubsystem>()->AddPlayerGlyphInput(GlyphDown);
	}
}

void AGlyphWeaverPlayerController::DPadLeft(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		GetGameInstance()->GetSubsystem<UGlyphWeaverSubsystem>()->AddPlayerGlyphInput(GlyphLeft);
	}
}

void AGlyphWeaverPlayerController::DPadRight(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		GetGameInstance()->GetSubsystem<UGlyphWeaverSubsystem>()->AddPlayerGlyphInput(GlyphRight);
	}
}