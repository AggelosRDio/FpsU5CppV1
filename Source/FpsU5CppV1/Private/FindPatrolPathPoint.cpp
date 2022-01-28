// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPatrolPathPoint.h"
#include "NPC_AiController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NPC.h"
#include "PatrolPath.h"
#include "FpsU5CppV1/BlackboardKeys.h"
//#include "BlackboardKeys"

UFindPatrolPathPoint::UFindPatrolPathPoint(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Patrol Path Point");
}

EBTNodeResult::Type UFindPatrolPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get the AI controller

	auto const controller = Cast<ANPC_AiController>(OwnerComp.GetAIOwner());

	// get current patrol path index from the blackboard
	auto index = OwnerComp.GetBlackboardComponent()->GetValueAsInt(BlackboardKeys::PatrolPathIndex);

	// use the index to get the current patrol path from the NPC 's reference to the patrol path
	auto const npc = Cast<ANPC>(controller->GetPawn());

	FVector const point = npc->GetPatrolPath()->GetPatrolPoint(index);

	// transform this point to a global position using its parent
	FVector const globalPoint = npc->GetPatrolPath()->GetActorTransform().TransformPosition(point);

	// write the current global path point to the blackboard.
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(BlackboardKeys::PatrolPathVector, globalPoint);

	// finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;

	//return Super::ExecuteTask(OwnerComp, NodeMemory);
}
