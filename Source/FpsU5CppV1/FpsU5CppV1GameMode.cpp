// Copyright Epic Games, Inc. All Rights Reserved.

#include "FpsU5CppV1GameMode.h"
#include "FpsU5CppV1HUD.h"
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
