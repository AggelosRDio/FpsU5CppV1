// Fill out your copyright notice in the Description page of Project Settings.


#include "MissileAttack.h"
#include "AIController.h"

UMissileAttack::UMissileAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Missile Attack");
}

EBTNodeResult::Type UMissileAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the NPC
	auto const controller = OwnerComp.GetAIOwner();


	if (auto const npc = Cast<ANPC>(controller->GetPawn()))
	{
		if (MontageHasFinished(npc))
		{
			npc->MissileAttack();
		}
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

bool UMissileAttack::MontageHasFinished(ANPC* const npc)
{
	return npc->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(npc->GetMontage());
}
