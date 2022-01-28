// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasePlayer.h"

#include "NPC_AiController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "FpsU5CppV1/BlackboardKeys.h"

UChasePlayer::UChasePlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get TargetLocation from blackboard via AI controller
	auto const controller = Cast<ANPC_AiController>(OwnerComp.GetAIOwner());
	auto const playerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

	// move to the players location
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(controller, playerLocation);

	// finish with success
	constexpr auto result = EBTNodeResult::Succeeded;
	FinishLatentTask(OwnerComp, result);
	return result;

	//return Super::ExecuteTask(OwnerComp, NodeMemory);
}
