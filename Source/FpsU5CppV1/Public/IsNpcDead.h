// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "IsNpcDead.generated.h"

/**
 * 
 */
UCLASS()
class FPSU5CPPV1_API UIsNpcDead : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UIsNpcDead();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
