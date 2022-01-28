// Fill out your copyright notice in the Description page of Project Settings.


#include "StopInvestigating.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FpsU5CppV1/BlackboardKeys.h"

UStopInvestigating::UStopInvestigating(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Stop Investigating");
}

EBTNodeResult::Type UStopInvestigating::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get the npc controller and write false to the blackboard key "IsInvestigating"
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BlackboardKeys::IsInvestigating, false);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
