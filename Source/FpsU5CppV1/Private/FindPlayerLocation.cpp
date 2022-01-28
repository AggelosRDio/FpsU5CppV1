// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "NPC_AiController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "FpsU5CppV1/BlackboardKeys.h"

UFindPlayerLocation::UFindPlayerLocation(FObjectInitializer const& objectInitializer)
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UFindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get player character and the NPC 's controller
	auto const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	// get player location to use as an origin

	FVector const playerLocation = player->GetActorLocation();

	if (searchRandom)
	{
		FNavLocation loc;

		//get the navigation system and generate a random location near the player
		auto const navSystem = UNavigationSystemV1::GetCurrent(GetWorld());

		if (navSystem->GetRandomPointInNavigableRadius(playerLocation, searchRadius, loc, nullptr))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(BlackboardKeys::targetLocation, loc.Location);
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BlackboardKeys::targetLocation, playerLocation);
	}

	// Finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
	//return Super::ExecuteTask(OwnerComp, NodeMemory);
}
