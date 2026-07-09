#pragma once

#include "CoreMinimal.h"
#include "Core/GlyphWeaverSubsystem.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SCompoundWidget.h"
#include "GlyphWeaverEditorTab.generated.h"

class STextBlock;
class SExpandableArea;
class UGlyphWeaverSubsystem;

USTRUCT()
struct FPuzzleSlotData
{
	GENERATED_BODY()
	
	FPuzzleSlotData()
	{
		OriginalOrderIndex = 0;
		ExpandableArea = nullptr;
		Name = nullptr;
	}
	
	FPrimaryAssetId PuzzleAssetId;
	
	int OriginalOrderIndex;
	
	TSharedPtr<SVerticalBox> MainWidget;
	
	TSharedPtr<SExpandableArea> ExpandableArea;
	
	TSharedPtr<STextBlock> Name;
};

class SGlyphWeaverEditorTab : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGlyphWeaverEditorTab)
	{
		
	}
	SLATE_END_ARGS()
	
	virtual ~SGlyphWeaverEditorTab() override;
	
	void Construct(const FArguments& InArgs);
	
private:
	void OnPostPIEStarted(const bool InIsSimulating);
	void OnEndPIE(const bool InIsSimulating);
	void TryToBindToRuntimeSubsystem();
	
	void InitializeMainSlot();
	void AddPuzzleSlot(const FPuzzleData& InPuzzleData);	
	void UpdateCurrentPuzzleSlot();
	void RestorePuzzleSlot(const FPuzzleSlotData& InPuzzleSlotData) const;
	void MovePuzzleSlotToTop(const FPuzzleSlotData& InPuzzleSlotData) const;
	
	const FTextBlockStyle& GetPuzzleTextStyle(bool InIsCurrentSlot) const;
	
	UWorld* GetWorldFromEditor();
	
	FDelegateHandle EndPIEHandle;
	FDelegateHandle PostPIEStartedHandle;
	
	TSharedPtr<SVerticalBox> PuzzlesVerticalBox;
	
	TWeakObjectPtr<UGlyphWeaverSubsystem> CachedRuntimeSubsystem;
	
	TMap<FPrimaryAssetId, FPuzzleSlotData> PuzzleSlots;
	
	FPrimaryAssetId CurrentPuzzleSlotAssetId;
};