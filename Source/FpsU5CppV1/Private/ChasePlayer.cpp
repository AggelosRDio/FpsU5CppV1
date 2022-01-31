// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasePlayer.h"

#include "NPC_AiController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "FpsU5CppV1/BlackboardKeys.h"
#include "NPC.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

UChasePlayer::UChasePlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// if player is in melee range, don't move
	//auto const isPlayerInMeleeRange = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BlackboardKeys::PlayerIsInMeleeRange);

	//if (isPlayerInMeleeRange) {
	//	// finish with success
	//	return FinishTaskWithSuccess(OwnerComp);
	//	/*constexpr auto result = EBTNodeResult::Succeeded;
	//	FinishLatentTask(OwnerComp, result);
	//	return result;*/
	//}

	// get TargetLocation from blackboard via AI controller
	auto const controller = Cast<ANPC_AiController>(OwnerComp.GetAIOwner());

	auto const playerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
	
	// move to the players location
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(controller, playerLocation);

	// Finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
	// finish with success
	//return FinishTaskWithSuccess(OwnerComp);

	//return Super::ExecuteTask(OwnerComp, NodeMemory);
}

EBTNodeResult::Type UChasePlayer::FinishTaskWithSuccess(UBehaviorTreeComponent& OwnerComp)
{
	constexpr auto result = EBTNodeResult::Succeeded;
	FinishLatentTask(OwnerComp, result);
	return result;
}
