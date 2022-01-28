// Fill out your copyright notice in the Description page of Project Settings.


#include "IncrementPathIndex.h"

#include "NPC.h"
#include "NPC_AiController.h"
#include "PatrolPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FpsU5CppV1/BlackboardKeys.h"

UIncrementPathIndex::UIncrementPathIndex(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Increment Path Index");
}

EBTNodeResult::Type UIncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get AI Controller
	const ANPC_AiController* const controller = Cast<ANPC_AiController>(OwnerComp.GetAIOwner());
	ANPC* const npc = Cast<ANPC>(controller->GetPawn());
	int const noOfPoints = npc->GetPatrolPath()->Num();

	int constexpr minIndex = 0;
	int const maxIndex = noOfPoints - 1;

	// get and set the blackboard index key
	int index = OwnerComp.GetBlackboardComponent()->GetValueAsInt(BlackboardKeys::PatrolPathIndex);

	if (bidirectional)
	{
		if (index >= maxIndex && Direction == EDirectionType::Forward)
			Direction = EDirectionType::Reverse;

		if (index == minIndex && Direction == EDirectionType::Reverse)
			Direction = EDirectionType::Forward;
	}
	
	auto const newIndexValue = (Direction == EDirectionType::Forward ? std::abs(++index) : std::abs(--index)) % noOfPoints;
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(BlackboardKeys::PatrolPathIndex, newIndexValue);
	
	//finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
