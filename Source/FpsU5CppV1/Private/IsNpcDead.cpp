// Fill out your copyright notice in the Description page of Project Settings.

#include "IsNpcDead.h"
#include "NPC_AiController.h"
#include "NPC.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FpsU5CppV1/BlackboardKeys.h"

UIsNpcDead::UIsNpcDead()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Npc Dead");
}

void UIsNpcDead::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	// get NPC
	auto const controller = Cast<ANPC_AiController>(OwnerComp.GetAIOwner());
	auto const npc = Cast<ANPC>(controller->GetPawn());

	float const npcHealth = npc->GetHealth();

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BlackboardKeys::IsDead, npcHealth <= 0.0f);

	//write true or false if the player is within melee range
	//OwnerComp.GetBlackboardComponent()->SetValueAsBool(BlackboardKeys::PlayerIsInMeleeRange, npc->GetDistanceTo(playerCharacter) <= meleeRange);


}