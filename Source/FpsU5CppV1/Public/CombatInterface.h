#pragma once

#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

UINTERFACE(Blueprintable)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class ICombatInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Combat)
		int MeleeAttack();
};