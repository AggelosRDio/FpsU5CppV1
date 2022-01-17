// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "FindPatrolPathPoint.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FPSU5CPPV1_API UFindPatrolPathPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UFindPatrolPathPoint(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
