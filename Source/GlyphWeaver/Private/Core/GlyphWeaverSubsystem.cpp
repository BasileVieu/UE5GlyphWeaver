#include "Core/GlyphWeaverSubsystem.h"
#include "Core/GlyphWeaverUtils.h"
#include "Data/GlyphDataAsset.h"
#include "Data/GlyphPuzzleDataAsset.h"
#include "Data/GlyphSequenceDataAsset.h"
#include "Recognition/GlyphMatcher.h"
#include "GlyphWeaver.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Puzzle/GlyphPuzzleActor.h"
#include "Save/GlyphWeaverSaveGame.h"

void UGlyphWeaverSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	GuessGlyphSequence.Name = "Guess Sequence";
	
	Load();
	
	RetrieveSaveGameData();
}

void UGlyphWeaverSubsystem::RegisterPuzzleActor(AGlyphPuzzleActor* InActor, const UGlyphPuzzleDataAsset* InPuzzleDataAsset)
{
	FPrimaryAssetId AssetId = InPuzzleDataAsset->GetPrimaryAssetId();
	
	if (!Puzzles.Contains(AssetId))
	{
		Puzzles.Add(AssetId, FPuzzleData(InActor, false));
	}
	
	Puzzles[InPuzzleDataAsset->GetPrimaryAssetId()].Actor = InActor;
	
	InitializePuzzleActor(InActor, InPuzzleDataAsset);
}

void UGlyphWeaverSubsystem::InitializePuzzleActor(AGlyphPuzzleActor* InActor, const UGlyphPuzzleDataAsset* InPuzzleDataAsset)
{
	if (IsPuzzleSolved(InPuzzleDataAsset))
	{
		InActor->Hide();
	}
	else
	{
		InActor->UnHide();
	}
}

FGlyphPuzzle UGlyphWeaverSubsystem::CreatePuzzle(const UGlyphPuzzleDataAsset* InPuzzleDataAsset)
{
	FGlyphPuzzle NewPuzzle;
	
	NewPuzzle.PrimaryAssetId = InPuzzleDataAsset->GetPrimaryAssetId();
	NewPuzzle.Name = InPuzzleDataAsset->PuzzleName;
	UGlyphSequenceDataAsset* SequenceDataAsset = InPuzzleDataAsset->SequenceDataAsset.LoadSynchronous();
	NewPuzzle.Sequence = SequenceDataAsset->CreateGlyphSequence();
	NewPuzzle.Solved = false;
	NewPuzzle.Rules = InPuzzleDataAsset->Rules;
	CurrentResetTimer = InPuzzleDataAsset->ResetTimer;
	
	return NewPuzzle;
}

void UGlyphWeaverSubsystem::DetectPuzzle(const APlayerController* InPlayerController, const UGlyphPuzzleDataAsset* InPuzzleDataAsset)
{
	if (CachedWorld.IsValid())
	{
		return;
	}
	
	CurrentPuzzle = CreatePuzzle(InPuzzleDataAsset);
	
	CachedWorld = InPlayerController->GetWorld();
	
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
	
	UGlyphWeaverUtils::PrintPuzzle(CurrentPuzzle);
}

void UGlyphWeaverSubsystem::UnDetectPuzzle(const APlayerController* InPlayerController)
{
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = InPlayerController->GetLocalPlayer()->GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>();
	
	UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(InPlayerController->InputComponent);
	
	if (CachedEnhancedSubsystem != InputSubsystem
		|| CachedEnhancedComponent != InputComponent)
	{
		return;
	}
	
	RemoveGuessGlyphsInputs();
	
	CurrentPuzzle = {};
	
	CachedWorld = nullptr;
	
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
	if (!CachedWorld.IsValid())
	{
		return;
	}

	if (!CurrentPuzzle.Sequence.ContainsGlyph(InPlayerGlyph))
	{
		RemoveGuessGlyphsInputs();
	}

	OnGuessGlyphAdded.Broadcast(InPlayerGlyph);

	GuessGlyphSequence.Add(InPlayerGlyph);
	
	CachedWorld->GetTimerManager().SetTimer(CurrentTimerHandle,
		this, &UGlyphWeaverSubsystem::RemoveGuessGlyphsInputs, CurrentResetTimer, false);
	
	UGlyphWeaverUtils::PrintSequence(GuessGlyphSequence);
	
	if (GlyphMatcher->Matches(CurrentPuzzle.Sequence, GuessGlyphSequence, CurrentPuzzle.Sequence.GetMaxValue(), CurrentPuzzle.Rules))
	{
		ValidateCurrentPuzzle();
		
		UE_LOG(LogGlyphWeaver, Log, TEXT("Current puzzle validated"));
	}
}

void UGlyphWeaverSubsystem::RemoveGuessGlyphsInputs()
{
	if (!CachedWorld.IsValid())
	{
		return;
	}
	
	OnGuessGlyphSequenceModified.Broadcast(GuessGlyphSequence);
	
	GuessGlyphSequence.Empty();
	
	CurrentResetTimer = 0.0f;
	
	GetWorld()->GetTimerManager().ClearTimer(CurrentTimerHandle);
}

void UGlyphWeaverSubsystem::PausePuzzleTimer(bool InIsPaused) const
{
	if (!CachedWorld.IsValid())
	{
		return;
	}
	
	if (InIsPaused)
	{
		CachedWorld->GetTimerManager().PauseTimer(CurrentTimerHandle);
	}
	else
	{
		CachedWorld->GetTimerManager().UnPauseTimer(CurrentTimerHandle);
	}
}

void UGlyphWeaverSubsystem::ValidatePuzzle(FPrimaryAssetId InPrimaryAssetId)
{
	Puzzles[InPrimaryAssetId].Actor->Hide();
	Puzzles[InPrimaryAssetId].Solved = true;
}

void UGlyphWeaverSubsystem::ResetPuzzle(FPrimaryAssetId InPrimaryAssetId)
{
	Puzzles[InPrimaryAssetId].Actor->UnHide();
	Puzzles[InPrimaryAssetId].Solved = false;
}

void UGlyphWeaverSubsystem::ResetAllPuzzles()
{
	for (TTuple Puzzle : Puzzles)
	{
		ResetPuzzle(Puzzle.Key);
	}
}

void UGlyphWeaverSubsystem::PlayerInputTriggered(const UGlyphDataAsset* InGlyphDataAsset, const FInputActionValue& InValue)
{
	if (InValue.Get<bool>())
	{
		AddGuessGlyphInput(InGlyphDataAsset->CreateGlyph());
	}
}

void UGlyphWeaverSubsystem::ValidateCurrentPuzzle()
{
	ValidatePuzzle(CurrentPuzzle.PrimaryAssetId);
	
	RemoveGuessGlyphsInputs();	
}

void UGlyphWeaverSubsystem::RetrieveSaveGameData()
{
	Puzzles.Empty();

	for (TTuple Puzzle : SaveGame->PuzzlesSaved)
	{
		Puzzles.Add(Puzzle.Key, FPuzzleData(nullptr, Puzzle.Value));
	}
}

void UGlyphWeaverSubsystem::ApplySaveGameData()
{
	for (TTuple Puzzle : Puzzles)
	{
		if (Puzzle.Value.Actor.IsValid())
		{
			InitializePuzzleActor(Puzzle.Value.Actor.Get(), Cast<UGlyphPuzzleDataAsset>(UAssetManager::Get().GetPrimaryAssetObject(Puzzle.Key)));
		}
	}
}

bool UGlyphWeaverSubsystem::IsPuzzleSolved(const FGlyphPuzzle& InPuzzle)
{
	return Puzzles.Contains(InPuzzle.PrimaryAssetId)
		&& Puzzles[InPuzzle.PrimaryAssetId].Solved;
}

bool UGlyphWeaverSubsystem::IsPuzzleSolved(const UGlyphPuzzleDataAsset* InPuzzleDataAsset)
{
	return Puzzles.Contains(InPuzzleDataAsset->GetPrimaryAssetId())
		&& Puzzles[InPuzzleDataAsset->GetPrimaryAssetId()].Solved;
}

void UGlyphWeaverSubsystem::Save()
{
	if (SaveGame == nullptr)
	{
		SaveGame = Cast<UGlyphWeaverSaveGame>(UGameplayStatics::CreateSaveGameObject(UGlyphWeaverSaveGame::StaticClass()));
	}
	
	SaveGame->PuzzlesSaved.Empty();
	
	for (TTuple Puzzle : Puzzles)
	{
		if (Puzzle.Value.Solved)
		{
			SaveGame->PuzzlesSaved.Add(Puzzle.Key, Puzzle.Value.Solved);
		}
	}
	
	UGameplayStatics::SaveGameToSlot(SaveGame, "GlyphWeaver", 0);
}

void UGlyphWeaverSubsystem::Load()
{
	SaveGame = Cast<UGlyphWeaverSaveGame>(UGameplayStatics::LoadGameFromSlot("GlyphWeaver", 0));
	
	if (SaveGame == nullptr)
	{
		SaveGame = Cast<UGlyphWeaverSaveGame>(UGameplayStatics::CreateSaveGameObject(UGlyphWeaverSaveGame::StaticClass()));
	}
}

/*EDITOR AVEC DEBUG TEMPS REEL*/