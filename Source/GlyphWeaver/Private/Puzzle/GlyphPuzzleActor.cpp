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
}

void AGlyphPuzzleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGlyphPuzzleActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	APawn* Pawn = Cast<APawn>(OtherActor);

	if (!Pawn)
	{
		return;
	}

	AController* Controller = Pawn->GetController();

	if (!Controller)
	{
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	if (PlayerController)
	{
		GetGameInstance()->GetSubsystem<UGlyphWeaverSubsystem>()->SetupPuzzle(PlayerController, GlyphPuzzleDataAsset);
	}
}

void AGlyphPuzzleActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* Pawn = Cast<APawn>(OtherActor);

	if (!Pawn)
	{
		return;
	}

	AController* Controller = Pawn->GetController();

	if (!Controller)
	{
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	if (PlayerController)
	{
		GetGameInstance()->GetSubsystem<UGlyphWeaverSubsystem>()->RemovePuzzle(PlayerController);
	}
}