#pragma once

#include "ActorStructs.generated.h"

USTRUCT(BlueprintType)
struct FActorIntStats
{
    GENERATED_BODY()

        //~ The following member variable will be accessible by Blueprint Graphs:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
        int Value;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
        int MaxValue;
};