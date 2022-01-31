// Fill out your copyright notice in the Description page of Project Settings.

#include "IsPlayerInMissileRange.h"
#include "NPC_AiController.h"
#include "NPC.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FpsU5CppV1/BlackboardKeys.h"

UIsPlayerInMissileRange::UIsPlayerInMissileRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player in Missile Range");
}

void UIsPlayerInMissileRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	// get NPC
	auto const controller = Cast<ANPC_AiController>(OwnerComp.GetAIOwner());
	auto const npc = Cast<ANPC>(controller->GetPawn());

	// Get player character
	auto const playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto const distanceToPlayer = npc->GetDistanceTo(playerCharacter);
	
	bool const isInMeleeRange = distanceToPlayer <= npc->GetMeleeRange();
	bool const isInMissileRange = distanceToPlayer <= npc->GetMissileRange();
	
	if (npc->GetCanAttackRanged()) 
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BlackboardKeys::PlayerIsInMissileRange, isInMissileRange);
	}
	//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BlackboardKeys::PlayerIsInMissileRange, );

	if (npc->GetCanAttackMelee())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BlackboardKeys::PlayerIsInMeleeRange, isInMeleeRange);

		if (!isInMeleeRange) return;

		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BlackboardKeys::PlayerIsInMissileRange, false);
	}
}