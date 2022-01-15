// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_AiController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "FpsU5CppV1/BlackboardKeys.h"
#include "FpsU5CppV1/FpsU5CppV1Character.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"


ANPC_AiController::ANPC_AiController(FObjectInitializer const& ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/AI/NPC_BT.NPC_BT'"));

	if (obj.Succeeded())
	{
		bTree = obj.Object;
	}

	BehaviourTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviourComp"));
	Blackboard = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, "BlackboardComp");

	SetupPerceptionSystem();
}

void ANPC_AiController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(bTree);
	BehaviourTreeComponent->StartTree(*bTree);
}

void ANPC_AiController::OnPossess(APawn* const InPawn)
{
	Super::OnPossess(InPawn);

	if(Blackboard)
	{
		Blackboard->InitializeBlackboard(*bTree->BlackboardAsset);
	}
}

UBlackboardComponent* ANPC_AiController::GetBlackboard() const
{
	return Blackboard;
}

void ANPC_AiController::OnUpdated(TArray<AActor*> const& updatedActors)
{

}

void ANPC_AiController::OnTargetDetected(AActor* actor, FAIStimulus const stimulus)
{
	if (auto const ch = Cast<AFpsU5CppV1Character>(actor))
	{
		GetBlackboard()->SetValueAsBool(BlackboardKeys::canSeePlayer, stimulus.WasSuccessfullySensed());
	}
}

void ANPC_AiController::SetupPerceptionSystem()
{
	// Create and initialise sight configuration object

	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SIght Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	sightConfig->SightRadius = 500.0f;
	sightConfig->LoseSightRadius = sightConfig->SightRadius + 50.0f;
	sightConfig->PeripheralVisionAngleDegrees = 90.0f;
	sightConfig->SetMaxAge(5.0f);
	sightConfig->AutoSuccessRangeFromLastSeenLocation = 900.0f;
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// add sight configuration component to perception component
	GetPerceptionComponent()->SetDominantSense(*sightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ANPC_AiController::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*sightConfig);
}
