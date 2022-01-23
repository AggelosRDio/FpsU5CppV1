// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DefaultGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FPSU5CPPV1_API UDefaultGameInstance : public UGameInstance //, public InGameMenuInterface
{
	GENERATED_BODY()

//
//public:
//
//	UPlayGroundGameInstance(const FObjectInitializer& ObjectInitializer);
//
//	virtual void Init();
//
//	FORCEINLINE class UInGameUI* GetInGameUI() const { return InGameUI; }
//
//	// Interface IInGameMenuInterface
//	// Method callable from blueprite to create the UI and add it to the viewport
//	UFUNCTION(BlueprintCallable)
//		void OnLoadInGameMenu() override;
//
//	void OnStartGamePath() override;
//
//	void OnStartGame() override;
//
//protected:
//	// Dynamic reference to the blueprint class
//	TSubclassOf<UUserWidget> InGameUIClass;
//
//	// Internal reference to the blueprint for gameplay logic
//	UPROPERTY(BlueprintReadWrite, Category = "UI")
//		class UInGameUI* InGameUI;
//
//
//	// Interface IInGameMenuInterface
//
//private:
//
//	// Reference to the Grid
//	class AGrid* CurrentGrid;
};
