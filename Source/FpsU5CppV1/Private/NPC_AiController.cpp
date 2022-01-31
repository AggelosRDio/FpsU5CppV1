// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_AiController.h"

#include "NPC.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "FpsU5CppV1/BlackboardKeys.h"
#include "Perception/AIPerceptionComponent.h"
#include "FpsU5CppV1/AiTags.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

ANPC_AiController::ANPC_AiController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void ANPC_AiController::BeginPlay()
{
	Super::BeginPlay();

	if (auto const NPC = Cast<ANPC>(GetPawn()))
	{
		if (auto const Tree = NPC->GetBehaviorTree())
		{
			UseBlackboard(Tree->BlackboardAsset, BlackboardComponent);
		}
	}
}

void ANPC_AiController::OnPossess(APawn* const InPawn)
{
	Super::OnPossess(InPawn);

	if(auto const NPC = Cast<ANPC>(InPawn))
	{
		if (auto const Tree = NPC->GetBehaviorTree())
			RunBehaviorTree(Tree);
			//..Blackboard->InitializeBlackboard(*bTree->BlackboardAsset);
	}
}

void ANPC_AiController::OnUpdated(TArray<AActor*> const& updatedActors)
{
	if (!BlackboardComponent) return;
	
	auto const npc = Cast<ANPC>(GetPawn());

	if (!npc) return;

	for(auto x = 0; x < updatedActors.Num(); x++)
	{
		FActorPerceptionBlueprintInfo info;
		GetPerceptionComponent()->GetActorsPerception(updatedActors[x], info);

		for (auto k = 0; k < info.LastSensedStimuli.Num(); k++)
		{
			FAIStimulus const stim = info.LastSensedStimuli[k];
			
			//if (stim.Tag == Tags::NoiseTag)
			//{
			//	//BlackboardComponent->SetValueAsBool(BlackboardKeys::IsInvestigating, stim.WasSuccessfullySensed());
			//	BlackboardComponent->SetValueAsVector(BlackboardKeys::targetLocation, stim.StimulusLocation);
			//}
			//else if (stim.Type.Name == "Default__AISense_Sight")
			//{
			//	BlackboardComponent->SetValueAsBool(BlackboardKeys::canSeePlayer, stim.WasSuccessfullySensed());
			//}
			
			if ((stim.Tag != Tags::NoiseTag) && (stim.Type.Name != "Default__AISense_Sight")) continue;
			
			bool const isSensed = stim.WasSuccessfullySensed();
			bool canSeePlayer = false;

			if (stim.Type.Name == "Default__AISense_Sight")
			{
				//	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Perception: Sight Sensed: ")  );
				//	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::FromInt(isSensed ? 1 : 0));
				BlackboardComponent->SetValueAsBool(BlackboardKeys::canSeePlayer, isSensed);
				canSeePlayer = isSensed;
			}

			if (stim.Tag == Tags::NoiseTag) {
				BlackboardComponent->SetValueAsVector(BlackboardKeys::targetLocation, stim.StimulusLocation);
			}

			if (!isSensed) continue;

			bool isAwake = npc->GetIsAwake();

			if (isSensed && !npc->GetIsAwake())
			{
				npc->SetIsAwake(true);
			}

			BlackboardComponent->SetValueAsObject(BlackboardKeys::Player, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			BlackboardComponent->SetValueAsBool(BlackboardKeys::IsAwake, npc->GetIsAwake());

			

			//// if the NPC can see the player character;
			//if (stim.Type.Name == "Default__AISense_Sight")
			//{
			//	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Perception: Sight Sensed: ")  );
			//	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::FromInt(isSensed ? 1 : 0));
			//	BlackboardComponent->SetValueAsBool(BlackboardKeys::canSeePlayer, isSensed);
			//	canSeePlayer = isSensed;
			//}
			//
			//if (!isSensed) return;

			//
			//auto const isAwake = BlackboardComponent->GetValueAsBool(BlackboardKeys::IsAwake);

			//if (!isAwake)
			//{
			//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Perception: Waking Up"));

			//	BlackboardComponent->SetValueAsBool(BlackboardKeys::IsAwake, true);
			//}

			//if (stim.Tag == Tags::NoiseTag)
			//{
			//	if (!canSeePlayer)
			//		BlackboardComponent->SetValueAsVector(BlackboardKeys::targetLocation, stim.StimulusLocation);
			//}

			//=========================================================================================================

			// if (!stim.WasSuccessfullySensed()) continue;
			/*if (isSensed)
				BlackboardComponent->SetValueAsVector(BlackboardKeys::targetLocation, stim.StimulusLocation);*/

			

			/*if(BlackboardComponent && stim.Tag == Tags::NoiseTag)
			{
				BlackboardComponent->SetValueAsBool(BlackboardKeys::IsInvestigating, stim.WasSuccessfullySensed());
				BlackboardComponent->SetValueAsVector(BlackboardKeys::targetLocation, stim.StimulusLocation);
			}
			else if(BlackboardComponent && stim.Type.Name == "Default__AISense_Sight")
			{
				BlackboardComponent->SetValueAsBool(BlackboardKeys::canSeePlayer, stim.WasSuccessfullySensed());
			}*/
		}
	}
}



void ANPC_AiController::SetupPerceptionSystem()
{
	// Create and initialise sight configuration object
	
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	if (sightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

		sightConfig->SightRadius = GetSightRadius();//500.0f;
		sightConfig->LoseSightRadius = GetLoseSightRadius();
		sightConfig->PeripheralVisionAngleDegrees = GetPeripheralVisionAngleDegrees();
		sightConfig->SetMaxAge(GetMaxAge());
		sightConfig->AutoSuccessRangeFromLastSeenLocation = GetAutoSuccessRangeFromLastSeenLocation();
		sightConfig->DetectionByAffiliation.bDetectEnemies = GetSightDetectEnemies();
		sightConfig->DetectionByAffiliation.bDetectFriendlies = GetSightDetectFriendlies();
		sightConfig->DetectionByAffiliation.bDetectNeutrals = GetSightDetectNeutrals();

		// add sight configuration component to perception component
		GetPerceptionComponent()->SetDominantSense(*sightConfig->GetSenseImplementation());
		//GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ANPC_AiController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense(*sightConfig);
	}

	// create and initialize hearing config object
	hearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));

	if (!hearingConfig) return;

	hearingConfig->HearingRange = GetHearingRange();
	hearingConfig->DetectionByAffiliation.bDetectEnemies = GetHearingDetectEnemies();
	hearingConfig->DetectionByAffiliation.bDetectFriendlies = GetHearingDetectFriendlies();
	hearingConfig->DetectionByAffiliation.bDetectNeutrals = GetHearingDetectNeutrals();

	// add sight configuration component to perception component
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ANPC_AiController::OnUpdated);
	GetPerceptionComponent()->ConfigureSense(*hearingConfig);
}

//void ANPC_AiController::SetupPerceptionSystem()
//{
//	// Create and initialise sight configuration object
//
//	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
//
//	if (sightConfig)
//	{
//		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
//
//		sightConfig->SightRadius = 500.0f;
//		sightConfig->LoseSightRadius = sightConfig->SightRadius + 50.0f;
//		sightConfig->PeripheralVisionAngleDegrees = 90.0f;
//		sightConfig->SetMaxAge(5.0f);
//		sightConfig->AutoSuccessRangeFromLastSeenLocation = 900.0f;
//		sightConfig->DetectionByAffiliation.bDetectEnemies = true;
//		sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
//		sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
//
//		// add sight configuration component to perception component
//		GetPerceptionComponent()->SetDominantSense(*sightConfig->GetSenseImplementation());
//		//GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ANPC_AiController::OnTargetDetected);
//		GetPerceptionComponent()->ConfigureSense(*sightConfig);
//	}
//
//	// create and initialize hearing config object
//	hearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
//
//	if (!hearingConfig) return;
//
//	hearingConfig->HearingRange = 3000.0f;
//	hearingConfig->DetectionByAffiliation.bDetectEnemies = true;
//	hearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
//	hearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
//
//	// add sight configuration component to perception component
//	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ANPC_AiController::OnUpdated);
//	GetPerceptionComponent()->ConfigureSense(*hearingConfig);
//}
