#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GlyphPuzzleActor.generated.h"

class UGlyphPuzzleDataAsset;
class UBoxComponent;

UCLASS()
class GLYPHWEAVER_API AGlyphPuzzleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AGlyphPuzzleActor();
	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	USceneComponent* Root;
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxOverlap;

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere)
	UGlyphPuzzleDataAsset* GlyphPuzzleDataAsset;
};