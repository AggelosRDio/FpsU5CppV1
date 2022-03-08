#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "WeaponAmmoType.generated.h"

/**
 *
 */
UENUM(BlueprintType)
enum class UWeaponAmmoType : uint8
{
    Unknown = 0 UMETA(DisplayName = "Unknown"),
    Shell = 1 UMETA(DisplayName = "Shell"),
    Bullet = 2 UMETA(DisplayName = "Bullet"),
    Energy = 3 UMETA(DisplayName = "Energy"),
    Rocket = 4 UMETA(DisplayName = "Rocket"),
    Melee = 5 UMETA(DisplayName = "Melee")
    /*LEFT = 1  UMETA(DisplayName = "LEFT"),
    UP = 2     UMETA(DisplayName = "UP"),
    RIGHT = 3*/
};
