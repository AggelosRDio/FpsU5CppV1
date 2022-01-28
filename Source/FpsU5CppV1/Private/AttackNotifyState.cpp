// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackNotifyState.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Components/SkeletalMeshComponent.h"
#include "NPC.h"

void UAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (auto const character = Cast<ANPC>(MeshComp->GetOwner()))
		{
			character->AttackStart();
		}
	}

	//Super::NotifyBegin(MeshComp, Animation, TotalDuration);
}

//void UAttackNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
//	float FrameDeltaTime)
//{
//	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
//}

void UAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (auto const character = Cast<ANPC>(MeshComp->GetOwner()))
		{
			character->AttackEnd();
		}
	}

	//Super::NotifyEnd(MeshComp, Animation);
}
