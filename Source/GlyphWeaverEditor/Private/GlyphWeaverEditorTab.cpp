#include "GlyphWeaverEditorTab.h"
#include "Editor.h"
#include "GlyphWeaverEditorModule.h"
#include "Style/GlyphWeaverEditorSlateStyle.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SExpandableArea.h"
#include "Widgets/Text/STextBlock.h"

SGlyphWeaverEditorTab::~SGlyphWeaverEditorTab()
{
	if (CachedRuntimeSubsystem.IsValid())
	{
		CachedRuntimeSubsystem->OnRegisteredPuzzle.RemoveAll(this);
		CachedRuntimeSubsystem->OnCurrentPuzzleDataChanged.RemoveAll(this);
	}
	
	if (EndPIEHandle != FDelegateHandle())
	{
		FEditorDelegates::EndPIE.Remove(EndPIEHandle);
	}
	
	if (PostPIEStartedHandle != FDelegateHandle())
	{
		FEditorDelegates::PostPIEStarted.Remove(PostPIEStartedHandle);
	}
}

void SGlyphWeaverEditorTab::Construct(const FArguments& InArgs)
{
	EndPIEHandle = FEditorDelegates::EndPIE.AddRaw(this, &SGlyphWeaverEditorTab::OnEndPIE);
	PostPIEStartedHandle = FEditorDelegates::PostPIEStarted.AddRaw(this, &SGlyphWeaverEditorTab::OnPostPIEStarted);
	
	TryToBindToRuntimeSubsystem();
}

void SGlyphWeaverEditorTab::OnPostPIEStarted(const bool InIsSimulating)
{
	if (GetWorldFromEditor())
	{
		TryToBindToRuntimeSubsystem();
	}
}

void SGlyphWeaverEditorTab::OnEndPIE(const bool InIsSimulating)
{
	if (CachedRuntimeSubsystem.IsValid())
	{
		CachedRuntimeSubsystem->OnRegisteredPuzzle.RemoveAll(this);
		CachedRuntimeSubsystem->OnCurrentPuzzleDataChanged.RemoveAll(this);
		CachedRuntimeSubsystem = nullptr;
	}
	
	if (PuzzlesVerticalBox.IsValid())
	{
		PuzzlesVerticalBox->ClearChildren();
	}
	
	PuzzleSlots.Empty();
	CurrentPuzzleSlotAssetId = FPrimaryAssetId();
}

void SGlyphWeaverEditorTab::TryToBindToRuntimeSubsystem()
{
	UWorld* World = GetWorldFromEditor();
	
	if (World != nullptr)
	{
		UGlyphWeaverSubsystem* NewSubsystem = World->GetGameInstance()->GetSubsystem<UGlyphWeaverSubsystem>();
		
		if (NewSubsystem == CachedRuntimeSubsystem.Get())
		{
			return;
		}
		
		if (CachedRuntimeSubsystem.IsValid())
		{
			CachedRuntimeSubsystem->OnRegisteredPuzzle.RemoveAll(this);
			CachedRuntimeSubsystem->OnCurrentPuzzleDataChanged.RemoveAll(this);
		}
		
		CachedRuntimeSubsystem = NewSubsystem;
		
		if (CachedRuntimeSubsystem.IsValid())
		{
			CachedRuntimeSubsystem->OnRegisteredPuzzle.AddRaw(this, &SGlyphWeaverEditorTab::AddPuzzleSlot);
			CachedRuntimeSubsystem->OnCurrentPuzzleDataChanged.AddRaw(this, &SGlyphWeaverEditorTab::UpdateCurrentPuzzleSlot);
			
			InitializeMainSlot();
		}
	}
}

void SGlyphWeaverEditorTab::InitializeMainSlot()
{
	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(500)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.0f, 0.5f)
			[
				SNew(SBorder)
				.BorderBackgroundColor(FLinearColor::Red)
				.Padding(2.0f)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(10.0f)
					[
						SNew(STextBlock)
						.Text(FText::FromString(TEXT("Puzzles")))
						.Justification(ETextJustify::Center)
					]
					+ SVerticalBox::Slot()
					.FillHeight(1.0f)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.FillWidth(1.0f)
						[
							SAssignNew(PuzzlesVerticalBox, SVerticalBox)
						]
					]
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBorder)
				.BorderBackgroundColor(FLinearColor::Blue)
				.Padding(2.0f)
				[
					SNew(SScrollBox)
					+ SScrollBox::Slot()
					.Padding(5.0f)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.Padding(2.0f)
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("Test Log")))
							.Justification(ETextJustify::Center)
						]
					]
				]
			]
		]
	];
	
	for (const TTuple<FPrimaryAssetId, FPuzzleData>& Pair : CachedRuntimeSubsystem->GetPuzzles())
	{
		AddPuzzleSlot(Pair.Value);
	}
}

void SGlyphWeaverEditorTab::AddPuzzleSlot(const FPuzzleData& InPuzzleData)
{
	TSharedPtr<SVerticalBox> NewPuzzleSlot;
	TSharedPtr<SExpandableArea> ExpandableArea;
	TSharedPtr<STextBlock> NameTextBlock;

	PuzzlesVerticalBox->AddSlot()
	                  .AutoHeight()
	                  .Padding(5)
	[
		SAssignNew(NewPuzzleSlot, SVerticalBox)
		+ SVerticalBox::Slot()
		[
			SAssignNew(ExpandableArea, SExpandableArea)
			.InitiallyCollapsed(true)
			.Padding(8.0f)
			.HeaderContent()
			[
				SNew(STextBlock)
				.Text(FText::FromString(InPuzzleData.Puzzle.Name.ToString()))
			]
			.BodyContent()
			[
				SNew(SBorder)
				.Padding(10.0f)
				.BorderBackgroundColor(FLinearColor::Gray)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.FillWidth(1.0f)
						[
							SNew(SBox)
							.MaxDesiredWidth(200.0f)
							[
								SAssignNew(NameTextBlock, STextBlock)
								.Text(FText::FromString(InPuzzleData.Puzzle.Name.ToString()))
								.TextStyle(&GetPuzzleTextStyle(false))
								.Justification(ETextJustify::Center)
								.AutoWrapText(true)
							]
						]
					]
				]
			]
		]
	];

	FPuzzleSlotData NewPuzzleSlotData;
	NewPuzzleSlotData.PuzzleAssetId = InPuzzleData.AssetId;
	NewPuzzleSlotData.OriginalOrderIndex = PuzzleSlots.Num();
	NewPuzzleSlotData.MainWidget = NewPuzzleSlot;
	NewPuzzleSlotData.ExpandableArea = ExpandableArea;
	NewPuzzleSlotData.Name = NameTextBlock;

	PuzzleSlots.Add(InPuzzleData.AssetId, NewPuzzleSlotData);
}

void SGlyphWeaverEditorTab::UpdateCurrentPuzzleSlot()
{
	if (!PuzzlesVerticalBox.IsValid()
		|| !CachedRuntimeSubsystem.IsValid())
	{
		return;
	}

	const FPuzzleData* CurrentPuzzleData = CachedRuntimeSubsystem->GetCurrentPuzzleData();
	
	if (CurrentPuzzleData == nullptr)
	{
		if (CurrentPuzzleSlotAssetId.IsValid())
		{
			RestorePuzzleSlot(PuzzleSlots[CurrentPuzzleSlotAssetId]);
		}
		
		CurrentPuzzleSlotAssetId = FPrimaryAssetId();
	}
	else if (CurrentPuzzleData->AssetId != CurrentPuzzleSlotAssetId)
	{
		if (const FPuzzleSlotData* SlotData = PuzzleSlots.Find(CurrentPuzzleData->AssetId))
		{
			RestorePuzzleSlot(*SlotData);
		}
		
		CurrentPuzzleSlotAssetId = CurrentPuzzleData->AssetId;
		
		const FPuzzleSlotData* SlotData = PuzzleSlots.Find(CurrentPuzzleData->AssetId);
		
		if (SlotData == nullptr)
		{
			return;
		}
		
		MovePuzzleSlotToTop(*SlotData);
	}
}

void SGlyphWeaverEditorTab::RestorePuzzleSlot(const FPuzzleSlotData& InPuzzleSlotData) const
{
	if (!PuzzlesVerticalBox.IsValid()
		|| !InPuzzleSlotData.MainWidget.IsValid())
	{
		return;
	}
	
	const int32 RemovedIndex = PuzzlesVerticalBox->RemoveSlot(InPuzzleSlotData.MainWidget.ToSharedRef());
	
	if (RemovedIndex == INDEX_NONE)
	{
		return;
	}

	PuzzlesVerticalBox->InsertSlot(InPuzzleSlotData.OriginalOrderIndex)
	                  .AutoHeight()
	                  .Padding(5.0f)
		[InPuzzleSlotData.MainWidget.ToSharedRef()];
	
	if (InPuzzleSlotData.Name.IsValid())
	{
		InPuzzleSlotData.Name->SetTextStyle(&GetPuzzleTextStyle(false), true);
	}
}

void SGlyphWeaverEditorTab::MovePuzzleSlotToTop(const FPuzzleSlotData& InPuzzleSlotData) const
{
	PuzzlesVerticalBox->RemoveSlot(InPuzzleSlotData.MainWidget.ToSharedRef());

	PuzzlesVerticalBox->InsertSlot(0)
	                  .AutoHeight()
	                  .Padding(5.0f)
		[InPuzzleSlotData.MainWidget.ToSharedRef()];
	
	InPuzzleSlotData.Name->SetTextStyle(&GetPuzzleTextStyle(true), true);
	
	InPuzzleSlotData.ExpandableArea->SetExpanded(true);
}

const FTextBlockStyle& SGlyphWeaverEditorTab::GetPuzzleTextStyle(bool InIsCurrentSlot) const
{
	return InIsCurrentSlot
		       ? FGlyphWeaverEditorSlateStyle::Get().GetWidgetStyle<FTextBlockStyle>("CurrentPuzzleDataText")
		       : FGlyphWeaverEditorSlateStyle::Get().GetWidgetStyle<FTextBlockStyle>("PuzzleDataText");
}

UWorld* SGlyphWeaverEditorTab::GetWorldFromEditor()
{
	if (GEditor
		&& GEditor->GetPIEWorldContext())
	{
		return GEditor->GetPIEWorldContext()->World();
	}
	
	return nullptr;
}