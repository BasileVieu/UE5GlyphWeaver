#include "Core/GlyphWeaverSettings.h"

FOnGlyphWeaverSettingsChanged UGlyphWeaverSettings::OnSettingsChanged;

void UGlyphWeaverSettings::SetDebugEnabled(const bool InIsDebugEnabled)
{
	DebugEnabled = InIsDebugEnabled;
}

void UGlyphWeaverSettings::SetConsoleVerbosity(const EGlyphWeaverConsoleVerbosity InIsConsoleVerbosity)
{
	ConsoleVerbosity = InIsConsoleVerbosity;
}

bool UGlyphWeaverSettings::GetDebugEnabled() const
{
	return DebugEnabled;
}

EGlyphWeaverConsoleVerbosity UGlyphWeaverSettings::GetConsoleVerbosity() const
{
	return ConsoleVerbosity;
}

#if WITH_EDITOR
void UGlyphWeaverSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (!PropertyChangedEvent.Property)
	{
		return;
	}
	
	const FName PropertyName = PropertyChangedEvent.Property->GetFName();
	
	OnSettingsChanged.Broadcast(PropertyName);
}
#endif