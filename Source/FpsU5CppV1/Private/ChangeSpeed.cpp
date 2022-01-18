// Fill out your copyright notice in the Description page of Project Settings.

#include "ChangeSpeed.h"
#include "AIController.h"
#include "NPC.h"
#include "GameFramework/CharacterMovementComponent.h"

UChangeSpeed::UChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	auto const controller = OwnerComp.GetAIOwner();
	ANPC* const npc = Cast<ANPC>(controller->GetPawn());
	npc->GetCharacterMovement()->MaxWalkSpeed = speed;
}

FString UChangeSpeed::GetStaticServiceDescription() const
{
	return FString("Change the NPC speed");
}
