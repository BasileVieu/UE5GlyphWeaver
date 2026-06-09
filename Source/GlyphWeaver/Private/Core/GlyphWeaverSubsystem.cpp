#include "Core/GlyphWeaverSubsystem.h"
#include "Core/GlyphWeaverUtils.h"
#include "Data/GlyphDataAsset.h"
#include "Data/GlyphPuzzleDataAsset.h"
#include "Data/GlyphSequenceDataAsset.h"
#include "Recognition/GlyphMatcher.h"
#include "GlyphWeaver.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

void UGlyphWeaverSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	GuessGlyphSequence.Name = "Guess Sequence";
}

void UGlyphWeaverSubsystem::SetupPuzzle(APlayerController* InPlayerController, const UGlyphPuzzleDataAsset* InPuzzleDataAsset)
{
	GlyphPuzzle.Name = InPuzzleDataAsset->PuzzleName;
	UGlyphSequenceDataAsset* SequenceDataAsset = InPuzzleDataAsset->SequenceDataAsset.LoadSynchronous();
	GlyphPuzzle.Sequence = SequenceDataAsset->CreateGlyphSequence();
	GlyphPuzzle.State = EGlyphPuzzleState::Inactive;
	GlyphPuzzle.Rules = InPuzzleDataAsset->Rules;
	
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = InPlayerController->GetLocalPlayer()->GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>();
	
	CachedEnhancedSubsystem = InputSubsystem;
	
	CachedEnhancedComponent = Cast<UEnhancedInputComponent>(InPlayerController->InputComponent);

	CachedEnhancedSubsystem->AddMappingContext(InPuzzleDataAsset->GlyphInputMapping, 100);
	
	for (TTuple Pair : InPuzzleDataAsset->GlyphsInputMap)
	{
		UInputAction* Action = Pair.Key;
		UGlyphDataAsset* GlyphDataAsset = Pair.Value.LoadSynchronous();
		
		FEnhancedInputActionEventBinding& Binding = CachedEnhancedComponent->BindActionInstanceLambda(Action, ETriggerEvent::Triggered, [this, GlyphDataAsset]
			(const FInputActionInstance& Instance)
		{
			PlayerInputTriggered(GlyphDataAsset, Instance.GetValue());
		});
		
		CachedEnhancedBindings.Add(Binding.GetHandle());
	}
	
	UGlyphWeaverUtils::PrintPuzzle(GlyphPuzzle);
}

void UGlyphWeaverSubsystem::RemovePuzzle(APlayerController* InPlayerController)
{
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = InPlayerController->GetLocalPlayer()->GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>();
	
	UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(InPlayerController->InputComponent);
	
	if (CachedEnhancedSubsystem != InputSubsystem
		|| CachedEnhancedComponent != InputComponent)
	{
		return;
	}
	
	for (int Binding : CachedEnhancedBindings)
	{
		CachedEnhancedComponent->RemoveBindingByHandle(Binding);
	}
	
	CachedEnhancedSubsystem = nullptr;
	CachedEnhancedComponent = nullptr;
	CachedEnhancedBindings.Empty();
}

void UGlyphWeaverSubsystem::AddGuessGlyphInput(const FGlyph& InPlayerGlyph)
{
	UE_LOG(LogGlyphWeaver, Log, TEXT("Add Player Glyph"));
	
	if (!GlyphPuzzle.Sequence.ContainsGlyph(InPlayerGlyph))
	{
		return;
	}
	
	OnGuessGlyphAdded.Broadcast(InPlayerGlyph);
	
	GuessGlyphSequence.Add(InPlayerGlyph);
	
	GetWorld()->GetTimerManager().SetTimer(UpdateGuessGlyphsTimerHandle,
		this, &UGlyphWeaverSubsystem::RemoveGuessGlyphsInputs, 3.0f, true);
	
	UGlyphWeaverUtils::PrintSequence(GuessGlyphSequence);
	
	UE_LOG(LogGlyphWeaver, Log, TEXT("Matches %s"),
		GlyphMatcher->Matches(GlyphPuzzle.Sequence, GuessGlyphSequence, 4, GlyphPuzzle.Rules)
		? TEXT("True") : TEXT("False"));
}

void UGlyphWeaverSubsystem::RemoveGuessGlyphsInputs()
{
	UE_LOG(LogGlyphWeaver, Log, TEXT("Remove Player Glyph"));
	
	OnGuessGlyphSequenceModified.Broadcast(GuessGlyphSequence);
	
	GuessGlyphSequence.Empty();
	
	GetWorld()->GetTimerManager().ClearTimer(UpdateGuessGlyphsTimerHandle);
	
	UGlyphWeaverUtils::PrintSequence(GuessGlyphSequence);
}

void UGlyphWeaverSubsystem::SetPause(bool InIsPaused)
{
	if (InIsPaused)
	{
		GetWorld()->GetTimerManager().ClearTimer(UpdateGuessGlyphsTimerHandle);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(UpdateGuessGlyphsTimerHandle,
			this, &UGlyphWeaverSubsystem::RemoveGuessGlyphsInputs, 3.0f, true);
	}
}

void UGlyphWeaverSubsystem::PlayerInputTriggered(UGlyphDataAsset* InGlyphDataAsset, const FInputActionValue& InValue)
{
	if (InValue.Get<bool>())
	{
		AddGuessGlyphInput(InGlyphDataAsset->CreateGlyph());
	}
}