// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FpsU5CppV1Character.h"
#include "FpsU5CppV1GameMode.generated.h"

UENUM()
enum class EGamePlayState
{
	EPlaying, EGameOver, Unknown
};

UCLASS(minimalapi)
class AFpsU5CppV1GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFpsU5CppV1GameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	AFpsU5CppV1Character* MyCharacter;

	UFUNCTION(BlueprintPure, Category = Health)
		EGamePlayState GetCurrentState() const;

	void SetCurrentState(EGamePlayState state);

private:
	EGamePlayState CurrentState;
	void HandleNewState(EGamePlayState newState) const;
};



