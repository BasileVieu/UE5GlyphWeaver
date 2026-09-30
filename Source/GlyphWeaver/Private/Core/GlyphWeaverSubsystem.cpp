#include "Core/GlyphWeaverSubsystem.h"
#include "Data/GlyphDataAsset.h"
#include "Data/GlyphPuzzleDataAsset.h"
#include "Data/GlyphSequenceDataAsset.h"
#include "Recognition/GlyphMatcher.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "TimerManager.h"
#include "Core/GlyphWeaverLogger.h"
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
	
	if (PuzzleData.Loaded == false)
	{
		PuzzleData.AssetId = AssetId;
		PuzzleData.Puzzle = CreatePuzzle(InPuzzleDataAsset, PuzzleData.Solved);
		PuzzleData.Loaded = true;
	}
	
	PuzzleData.Actor = InActor;
	
	InitializePuzzleActor(InActor, &PuzzleData);
	
	OnRegisteredPuzzle.Broadcast(PuzzleData);
}

void UGlyphWeaverSubsystem::InitializePuzzleActor(AGlyphPuzzleActor* InActor, const FPuzzleData* InPuzzleData)
{
	if (IsPuzzleSolved(InPuzzleData))
	{
		InActor->PuzzleValidated();
	}
	else
	{
		InActor->PuzzleReset();
	}
}

FGlyphPuzzle UGlyphWeaverSubsystem::CreatePuzzle(const UGlyphPuzzleDataAsset* InPuzzleDataAsset, bool InSolved)
{
	FGlyphPuzzle NewPuzzle;
	
	NewPuzzle.PrimaryAssetId = InPuzzleDataAsset->GetPrimaryAssetId();
	NewPuzzle.Name = InPuzzleDataAsset->PuzzleName;
	UGlyphSequenceDataAsset* SequenceDataAsset = InPuzzleDataAsset->SequenceDataAsset.LoadSynchronous();
	NewPuzzle.Sequence = SequenceDataAsset->CreateGlyphSequence();
	NewPuzzle.Solved = InSolved;
	NewPuzzle.Rules = InPuzzleDataAsset->Rules;
	
	return NewPuzzle;
}

void UGlyphWeaverSubsystem::DetectPuzzle(const APlayerController* InPlayerController, const UGlyphPuzzleDataAsset* InPuzzleDataAsset)
{
	if (!IsValid(InPlayerController)
		|| !IsValid(InPuzzleDataAsset))
	{
		return;
	}
	
	if (CurrentPuzzleAssetId.IsValid())
	{
		return;
	}
	
	const FPrimaryAssetId AssetId = InPuzzleDataAsset->GetPrimaryAssetId();
	
	if (!AssetId.IsValid())
	{
		return;
	}
	
	FPuzzleData* CurrentPuzzleData = PuzzleDatas.Find(AssetId);
	
	if (CurrentPuzzleData == nullptr)
	{
		return;
	}
	
	ULocalPlayer* LocalPlayer = InPlayerController->GetLocalPlayer();
	
	if (!IsValid(LocalPlayer))
	{
		return;
	}
	
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	
	UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(InPlayerController->InputComponent);
	
	if (!IsValid(InputSubsystem)
		|| !IsValid(InputComponent))
	{
		return;
	}
	
	UWorld* World = InPlayerController->GetWorld();
	
	if (!IsValid(World))
	{
		return;
	}
	
	CurrentPuzzleAssetId = AssetId;
	CurrentResetTimer = InPuzzleDataAsset->ResetTimer;
	CachedWorld = InPlayerController->GetWorld();
	CachedEnhancedSubsystem = InputSubsystem;
	CachedEnhancedComponent = InputComponent;
	CachedInputMappingContext = InPuzzleDataAsset->GlyphInputMapping;
	CachedEnhancedSubsystem->AddMappingContext(CachedInputMappingContext, 100);	
	CachedEnhancedBindings.Empty();
	
	for (TTuple Pair : InPuzzleDataAsset->GlyphsInputMap)
	{
		UInputAction* Action = Pair.Key;
		UGlyphDataAsset* GlyphDataAsset = Pair.Value.LoadSynchronous();
		
		FEnhancedInputActionEventBinding& Binding = CachedEnhancedComponent->BindActionInstanceLambda(
			Action,
			ETriggerEvent::Triggered,
			[this, GlyphDataAsset](const FInputActionInstance& Instance)
		{
			PlayerInputTriggered(GlyphDataAsset, Instance.GetValue());
		});
		
		CachedEnhancedBindings.Add(Binding.GetHandle());
	}
	
	OnCurrentPuzzleDataChanged.Broadcast();
}

void UGlyphWeaverSubsystem::UnDetectPuzzle(const APlayerController* InPlayerController)
{
	if (!IsValid(InPlayerController))
	{
		return;
	}
	
	const ULocalPlayer* LocalPlayer = InPlayerController->GetLocalPlayer();
	
	if (!IsValid(LocalPlayer))
	{
		return;
	}
	
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		InPlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	
	UEnhancedInputComponent* InputComponent =
		Cast<UEnhancedInputComponent>(InPlayerController->InputComponent);
	
	if (!IsValid(InputSubsystem)
		|| !IsValid(InputComponent))
	{
		return;
	}
	
	if (CachedEnhancedSubsystem != InputSubsystem
		|| CachedEnhancedComponent != InputComponent)
	{
		return;
	}
	
	UEnhancedInputComponent* EnhancedComponent = CachedEnhancedComponent.Get();
	UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = CachedEnhancedSubsystem.Get();
	UInputMappingContext* InputMappingContext = CachedInputMappingContext.Get();
	
	CurrentPuzzleAssetId = FPrimaryAssetId();
	
	if (CachedWorld.IsValid())
	{
		CachedWorld->GetTimerManager().ClearTimer(CurrentTimerHandle);
	}
	
	GuessGlyphSequence.Empty();
	CurrentResetTimer = 0.0f;

	if (IsValid(EnhancedComponent))
	{
		for (int Binding : CachedEnhancedBindings)
		{
			EnhancedComponent->RemoveBindingByHandle(Binding);
		}
	}

	CachedEnhancedBindings.Empty();
	
	if (IsValid(EnhancedSubsystem))
	{
		EnhancedSubsystem->RemoveMappingContext(InputMappingContext);
	}
	
	CachedInputMappingContext = nullptr;	
	CachedEnhancedSubsystem = nullptr;
	CachedEnhancedComponent = nullptr;	
	CachedWorld = nullptr;
	
	OnCurrentPuzzleDataChanged.Broadcast();
}

void UGlyphWeaverSubsystem::AddGuessGlyphInput(const FGlyph& InPlayerGlyph)
{
	if (!CurrentPuzzleAssetId.IsValid())
	{
		return;
	}
	
	FPuzzleData* CurrentPuzzleData = GetCurrentPuzzleData();
	
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
	
	if (GlyphMatcher->Matches(CurrentPuzzleData->Puzzle.Sequence, GuessGlyphSequence,
		CurrentPuzzleData->Puzzle.Sequence.GetMaxValue(), CurrentPuzzleData->Puzzle.Rules))
	{
		ValidateCurrentPuzzle();
	}
}

void UGlyphWeaverSubsystem::RemoveGuessGlyphsInputs()
{
	if (!CurrentPuzzleAssetId.IsValid())
	{
		return;
	}
	
	const FGlyphSequence PreviousSequence = GuessGlyphSequence;
	
	GuessGlyphSequence.Empty();
	
	CurrentResetTimer = 0.0f;
	
	if (CachedWorld.IsValid())
	{
		CachedWorld->GetTimerManager().ClearTimer(CurrentTimerHandle);
	}
	
	OnGuessGlyphSequenceModified.Broadcast(GuessGlyphSequence);
}

void UGlyphWeaverSubsystem::PausePuzzleTimer(bool InIsPaused) const
{
	if (!CurrentPuzzleAssetId.IsValid()
		|| !CachedWorld.IsValid())
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
	FPuzzleData* PuzzleData = PuzzleDatas.Find(InPuzzleAssetId);
	
	if (PuzzleData == nullptr)
	{
		return;
	}
	
	PuzzleData->Solved = true;
	
	if (PuzzleData->Actor.IsValid())
	{
		PuzzleData->Actor->PuzzleValidated();
	}
}

void UGlyphWeaverSubsystem::ResetPuzzle(const FPrimaryAssetId& InPuzzleAssetId)
{
	FPuzzleData* PuzzleData = PuzzleDatas.Find(InPuzzleAssetId);
	
	if (PuzzleData == nullptr)
	{
		return;
	}
	
	PuzzleData->Solved = false;
	
	if (PuzzleData->Actor.IsValid())
	{
		PuzzleData->Actor->PuzzleReset();
	}
}

const TMap<FPrimaryAssetId, FPuzzleData>& UGlyphWeaverSubsystem::GetPuzzles() const
{
	return PuzzleDatas;
}

FPuzzleData* UGlyphWeaverSubsystem::GetCurrentPuzzleData()
{
	return PuzzleDatas.Find(CurrentPuzzleAssetId);
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
	ValidatePuzzle(CurrentPuzzleAssetId);
	
	RemoveGuessGlyphsInputs();
}

void UGlyphWeaverSubsystem::RetrieveSaveGameData()
{
	PuzzleDatas.Empty();

	for (TTuple PuzzleSaved : SaveGame->PuzzlesSaved)
	{
		FPuzzleData NewPuzzleData;
		NewPuzzleData.AssetId = PuzzleSaved.Key;
		NewPuzzleData.Solved = PuzzleSaved.Value;
		NewPuzzleData.Loaded = false;
		
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