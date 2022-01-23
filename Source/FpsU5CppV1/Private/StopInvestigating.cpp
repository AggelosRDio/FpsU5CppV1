// Fill out your copyright notice in the Description page of Project Settings.


#include "StopInvestigating.h"
#include "NPC_AiController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FpsU5CppV1/BlackboardKeys.h"

UStopInvestigating::UStopInvestigating(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Stop Investigating");
}

EBTNodeResult::Type UStopInvestigating::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get the npc controller and write false to the blackboard key "IsInvestigating"
	auto const controller = Cast<ANPC_AiController>(OwnerComp.GetAIOwner());
	controller->GetBlackboard()->SetValueAsBool(BlackboardKeys::IsInvestigating, false);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
