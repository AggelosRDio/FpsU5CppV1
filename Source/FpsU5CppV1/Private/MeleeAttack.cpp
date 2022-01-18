// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeAttack.h"

#include "AIController.h"

UMeleeAttack::UMeleeAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UMeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the NPC
	auto const controller = OwnerComp.GetAIOwner();
	auto const npc = Cast<ANPC>(controller->GetPawn());

	if (ICombatInterface* const icombat = Cast<ICombatInterface>(npc))
	{
		if (MontageHasFinished(npc))
		{
			icombat->Execute_MeleeAttack(npc);
		}
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

bool UMeleeAttack::MontageHasFinished(ANPC* const npc)
{
	return npc->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(npc->GetMontage());
}
