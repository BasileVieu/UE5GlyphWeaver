#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GlyphPuzzleActor.generated.h"

class UGlyphPuzzleDataAsset;
class UBoxComponent;

/**
 * Actor used to represent the puzzle in the world including a box component to trigger the puzzle.
 */
UCLASS(Blueprintable)
class GLYPHWEAVER_API AGlyphPuzzleActor : public AActor
{
	GENERATED_BODY()
	
public:
	AGlyphPuzzleActor();
	
	UFUNCTION(BlueprintNativeEvent)
	void PuzzleValidated();
	
	UFUNCTION(BlueprintCallable)
	void PuzzleValidated_CPP();
	
	UFUNCTION(BlueprintNativeEvent)
	void PuzzleReset();
	
	UFUNCTION(BlueprintCallable)
	void PuzzleReset_CPP();
	
	UFUNCTION(BlueprintCallable)
	void Hide();
	
	UFUNCTION(BlueprintCallable)
	void UnHide();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* BoxOverlap;

protected:
	virtual void BeginPlay() override;
	
private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    	
	UPROPERTY(EditAnywhere, Category="GlyphWeaver")
	UGlyphPuzzleDataAsset* GlyphPuzzleDataAsset;
};