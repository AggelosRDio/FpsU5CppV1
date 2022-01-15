// Fill out your copyright notice in the Description page of Project Settings.


#include "FindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "NPC_AiController.h"
#include "FpsU5CppV1/BlackboardKeys.h"
//#include "BlackboardKeys.h"

UFindRandomLocation::UFindRandomLocation(FObjectInitializer const& objectInitializer)
{
	NodeName = TEXT("Find Random Location");
}

EBTNodeResult::Type UFindRandomLocation::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	// get AI Controller and its NPC
	auto const cont = Cast<ANPC_AiController>(ownerComp.GetAIOwner());
	auto const npc = cont->GetPawn();

	// obtain npc location to use as an origin location
	FVector const origin = npc->GetActorLocation();
	
	// get the navigation system and generate a random location on the NavMesh
	UNavigationSystemV1* const navSys = UNavigationSystemV1::GetCurrent(GetWorld());
	FNavLocation loc;

	if(navSys->GetRandomPointInNavigableRadius(origin, searchRadius, loc, nullptr))
	{
		cont->GetBlackboard()->SetValueAsVector(BlackboardKeys::targetLocation, loc.Location);
	}

	// finish with success
	FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
	//return Super::ExecuteTask(ownerComp, nodeMemory);
}
