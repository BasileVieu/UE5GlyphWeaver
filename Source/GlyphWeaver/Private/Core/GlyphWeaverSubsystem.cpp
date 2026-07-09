#include "Core/GlyphWeaverSubsystem.h"
#include "Core/GlyphWeaverUtils.h"
#include "Data/GlyphDataAsset.h"
#include "Data/GlyphPuzzleDataAsset.h"
#include "Data/GlyphSequenceDataAsset.h"
#include "Recognition/GlyphMatcher.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "TimerManager.h"
#include "Engine/AssetManager.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
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
	if (!InActor
		|| !InPuzzleDataAsset)
	{
		return;
	}
	
	const FPrimaryAssetId AssetId = InPuzzleDataAsset->GetPrimaryAssetId();
	
	FPuzzleData& PuzzleData = PuzzleDatas.FindOrAdd(AssetId);
	
	if (PuzzleData.Puzzle.Sequence.Glyphs.Num() == 0)
	{
		PuzzleData.AssetId = AssetId;
		PuzzleData.Puzzle = CreatePuzzle(InPuzzleDataAsset);
		PuzzleData.Solved = false;
	}
	
	PuzzleData.Actor = InActor;
	
	InitializePuzzleActor(InActor, &PuzzleData);
	
	OnRegisteredPuzzle.Broadcast(PuzzleData);
}

void UGlyphWeaverSubsystem::InitializePuzzleActor(AGlyphPuzzleActor* InActor, const FPuzzleData* InPuzzleData)
{
	if (IsPuzzleSolved(InPuzzleData))
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
	
	return NewPuzzle;
}

void UGlyphWeaverSubsystem::DetectPuzzle(const APlayerController* InPlayerController, const UGlyphPuzzleDataAsset* InPuzzleDataAsset)
{
	if (CurrentPuzzleData != nullptr)
	{
		return;
	}
	
	CurrentPuzzleData = PuzzleDatas.Find(InPuzzleDataAsset->GetPrimaryAssetId());
	
	if (CurrentPuzzleData == nullptr)
	{
		return;
	}
	
	CurrentResetTimer = InPuzzleDataAsset->ResetTimer;
	
	OnCurrentPuzzleDataChanged.Broadcast();
	
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
	
	UGlyphWeaverUtils::PrintPuzzle(CurrentPuzzleData->Puzzle);
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
	
	CurrentPuzzleData = nullptr;
	
	OnCurrentPuzzleDataChanged.Broadcast();
	
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
	if (CurrentPuzzleData == nullptr)
	{
		return;
	}

	if (!CurrentPuzzleData->Puzzle.Sequence.ContainsGlyph(InPlayerGlyph))
	{
		RemoveGuessGlyphsInputs();
	}

	OnGuessGlyphAdded.Broadcast(InPlayerGlyph);

	GuessGlyphSequence.Add(InPlayerGlyph);
	
	CachedWorld->GetTimerManager().SetTimer(CurrentTimerHandle,
		this, &UGlyphWeaverSubsystem::RemoveGuessGlyphsInputs, CurrentResetTimer, false);
	
	UGlyphWeaverUtils::PrintSequence(GuessGlyphSequence);
	
	if (GlyphMatcher->Matches(CurrentPuzzleData->Puzzle.Sequence, GuessGlyphSequence,
		CurrentPuzzleData->Puzzle.Sequence.GetMaxValue(), CurrentPuzzleData->Puzzle.Rules))
	{
		ValidateCurrentPuzzle();
	}
}

void UGlyphWeaverSubsystem::RemoveGuessGlyphsInputs()
{
	if (CurrentPuzzleData == nullptr)
	{
		return;
	}
	
	OnGuessGlyphSequenceModified.Broadcast(GuessGlyphSequence);
	
	GuessGlyphSequence.Empty();
	
	CurrentResetTimer = 0.0f;
	
	CachedWorld->GetTimerManager().ClearTimer(CurrentTimerHandle);
}

void UGlyphWeaverSubsystem::PausePuzzleTimer(bool InIsPaused) const
{
	if (CurrentPuzzleData == nullptr)
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

void UGlyphWeaverSubsystem::ValidatePuzzle(const FPrimaryAssetId& InPuzzleAssetId)
{
	PuzzleDatas[InPuzzleAssetId].Actor->Hide();
	PuzzleDatas[InPuzzleAssetId].Solved = true;
}

void UGlyphWeaverSubsystem::ResetPuzzle(const FPrimaryAssetId& InPuzzleAssetId)
{
	PuzzleDatas[InPuzzleAssetId].Actor->UnHide();
	PuzzleDatas[InPuzzleAssetId].Solved = false;
}

const TMap<FPrimaryAssetId, FPuzzleData>& UGlyphWeaverSubsystem::GetPuzzles() const
{
	return PuzzleDatas;
}

const FPuzzleData* UGlyphWeaverSubsystem::GetCurrentPuzzleData() const
{
	return CurrentPuzzleData;
}

void UGlyphWeaverSubsystem::ResetAllPuzzles()
{
	for (TTuple Puzzle : PuzzleDatas)
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
	ValidatePuzzle(CurrentPuzzleData->AssetId);
	
	RemoveGuessGlyphsInputs();
}

void UGlyphWeaverSubsystem::RetrieveSaveGameData()
{
	PuzzleDatas.Empty();

	for (TTuple PuzzleSaved : SaveGame->PuzzlesSaved)
	{
		UGlyphPuzzleDataAsset* DataAsset = Cast<UGlyphPuzzleDataAsset>(UAssetManager::Get().GetPrimaryAssetObject(PuzzleSaved.Key));
		
		FPuzzleData NewPuzzleData;
		NewPuzzleData.AssetId = PuzzleSaved.Key;
		NewPuzzleData.Puzzle = CreatePuzzle(DataAsset);
		NewPuzzleData.Solved = PuzzleSaved.Value;
		
		PuzzleDatas.Add(NewPuzzleData.AssetId, NewPuzzleData);
	}
}

void UGlyphWeaverSubsystem::ApplySaveGameData()
{
	for (TTuple Puzzle : PuzzleDatas)
	{
		if (Puzzle.Value.Actor.IsValid())
		{
			InitializePuzzleActor(Puzzle.Value.Actor.Get(), &Puzzle.Value);
		}
	}
}

bool UGlyphWeaverSubsystem::IsPuzzleSolved(const FPuzzleData* InPuzzleData)
{
	if (const FPuzzleData* Data = PuzzleDatas.Find(InPuzzleData->AssetId))
	{
		return Data->Solved;
	}
	
	return false;
}

void UGlyphWeaverSubsystem::Save()
{
	if (SaveGame == nullptr)
	{
		SaveGame = Cast<UGlyphWeaverSaveGame>(UGameplayStatics::CreateSaveGameObject(UGlyphWeaverSaveGame::StaticClass()));
	}
	
	SaveGame->PuzzlesSaved.Empty();
	
	for (TTuple Puzzle : PuzzleDatas)
	{
		if (Puzzle.Value.Solved)
		{
			SaveGame->PuzzlesSaved.Add(Puzzle.Value.AssetId, Puzzle.Value.Solved);
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