// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "AttackType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class UAttackType : uint8
{
    NOTATTACKING = 0 UMETA(DisplayName = "NotAttacking"),
    MELEE = 1 UMETA(DisplayName = "MeleeAttack"),
    MISSILE = 2 UMETA(DisplayName = "MissileAttack")
    /*LEFT = 1  UMETA(DisplayName = "LEFT"),
    UP = 2     UMETA(DisplayName = "UP"),
    RIGHT = 3*/
};
