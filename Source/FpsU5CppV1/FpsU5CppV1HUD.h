// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FpsU5CppV1HUD.generated.h"

UCLASS()
class AFpsU5CppV1HUD : public AHUD
{
	GENERATED_BODY()

public:
	AFpsU5CppV1HUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

