#include "GlyphPuzzle/GlyphPuzzleActor.h"
#include "Components/BoxComponent.h"

AGlyphPuzzleActor::AGlyphPuzzleActor()
{
 	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	RootComponent = Root;
	
	BoxOverlap = CreateDefaultSubobject<UBoxComponent>(FName("OverlapBox"));
	BoxOverlap->SetupAttachment(Root);
}

void AGlyphPuzzleActor::BeginPlay()
{
	Super::BeginPlay();
}

void AGlyphPuzzleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}