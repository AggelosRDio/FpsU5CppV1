// Copyright Epic Games, Inc. All Rights Reserved.

#include "FpsU5CppV1GameMode.h"
#include "FpsU5CppV1HUD.h"
#include "Kismet/GameplayStatics.h"
#include "FpsU5CppV1Character.h"
#include "UObject/ConstructorHelpers.h"

AFpsU5CppV1GameMode::AFpsU5CppV1GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFpsU5CppV1HUD::StaticClass();
}

void AFpsU5CppV1GameMode::BeginPlay()
{
	Super::BeginPlay();

	SetCurrentState(EGamePlayState::EPlaying);

	MyCharacter = Cast<AFpsU5CppV1Character>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void AFpsU5CppV1GameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (MyCharacter)
	{
		if(FMath::IsNearlyZero(MyCharacter->GetCurrentHealth(), 0.001f))
		{
			SetCurrentState(EGamePlayState::EGameOver);
		}
	}
}

EGamePlayState AFpsU5CppV1GameMode::GetCurrentState() const
{
	return CurrentState;
}

void AFpsU5CppV1GameMode::SetCurrentState(EGamePlayState state)
{
	CurrentState = state;
	HandleNewState(CurrentState);
}

void AFpsU5CppV1GameMode::HandleNewState(EGamePlayState newState) const
{
	switch (newState)
	{
	case EGamePlayState::EPlaying:
		{
			// Do Nothing
		}
		break;
	case EGamePlayState::EGameOver:
		{
			UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
		}
		break;
	case EGamePlayState::Unknown:
		{
			
		}
		break;
	default:
		{
			
		}
	}
}
