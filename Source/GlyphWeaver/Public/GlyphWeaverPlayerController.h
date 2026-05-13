#pragma once

#include "CoreMinimal.h"
#include "GlyphPuzzle/GlyphPuzzle.h"
#include "GameFramework/PlayerController.h"
#include "GlyphWeaverPlayerController.generated.h"

struct FInputActionValue;
class UGlyphPuzzleDataAsset;
class UGlyphDataAsset;
class UInputMappingContext;
class UInputAction;

UCLASS()
class GLYPHWEAVER_API AGlyphWeaverPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	void SetPause();
	
	void DPadUp(const FInputActionValue& Value);
	void DPadDown(const FInputActionValue& Value);
	void DPadLeft(const FInputActionValue& Value);
	void DPadRight(const FInputActionValue& Value);
	
private:
	UPROPERTY(EditAnywhere, Category="GlyphWeaver")
	TSoftObjectPtr<UGlyphPuzzleDataAsset> GlyphPuzzleDataAsset;
	
	UPROPERTY(EditAnywhere, Category="GlyphWeaver")
	TSoftObjectPtr<UGlyphDataAsset> PuzzleGlyphDataAssetUp;
	
	UPROPERTY(EditAnywhere, Category="GlyphWeaver")
	TSoftObjectPtr<UGlyphDataAsset> PuzzleGlyphDataAssetDown;
	
	UPROPERTY(EditAnywhere, Category="GlyphWeaver")
	TSoftObjectPtr<UGlyphDataAsset> PuzzleGlyphDataAssetLeft;
	
	UPROPERTY(EditAnywhere, Category="GlyphWeaver")
	TSoftObjectPtr<UGlyphDataAsset> PuzzleGlyphDataAssetRight;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* InputMappingFreeform;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_Pause;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_DPad_Up;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_DPad_Down;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_DPad_Left;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_DPad_Right;
	
	bool IsPaused = false;
	
	FGlyph GlyphUp;
	FGlyph GlyphDown;
	FGlyph GlyphLeft;
	FGlyph GlyphRight;
};