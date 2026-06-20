#include "Puzzle/GlyphPuzzleActor.h"
#include "Core/GlyphWeaverSubsystem.h"
#include "Components/BoxComponent.h"

AGlyphPuzzleActor::AGlyphPuzzleActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	RootComponent = Root;

	BoxOverlap = CreateDefaultSubobject<UBoxComponent>(FName("OverlapBox"));
	BoxOverlap->SetupAttachment(Root);
	BoxOverlap->SetGenerateOverlapEvents(true);
	BoxOverlap->OnComponentBeginOverlap.AddUniqueDynamic(this, &AGlyphPuzzleActor::OnOverlapBegin);
	BoxOverlap->OnComponentEndOverlap.AddUniqueDynamic(this, &AGlyphPuzzleActor::OnOverlapEnd);
}

void AGlyphPuzzleActor::BeginPlay()
{
	Super::BeginPlay();
	
	GetGameInstance()->GetSubsystem<UGlyphWeaverSubsystem>()->RegisterPuzzleActor(this, GlyphPuzzleDataAsset);
}

void AGlyphPuzzleActor::Hide()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}

void AGlyphPuzzleActor::UnHide()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}

void AGlyphPuzzleActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	const APawn* Pawn = Cast<APawn>(OtherActor);

	if (!Pawn)
	{
		return;
	}

	AController* Controller = Pawn->GetController();

	if (!Controller)
	{
		return;
	}

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		GetGameInstance()->GetSubsystem<UGlyphWeaverSubsystem>()->DetectPuzzle(PlayerController, GlyphPuzzleDataAsset);
	}
}

void AGlyphPuzzleActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const APawn* Pawn = Cast<APawn>(OtherActor);

	if (!Pawn)
	{
		return;
	}

	AController* Controller = Pawn->GetController();

	if (!Controller)
	{
		return;
	}

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		GetGameInstance()->GetSubsystem<UGlyphWeaverSubsystem>()->UnDetectPuzzle(PlayerController);
	}
}