// Fill out your copyright notice in the Description page of Project Settings.


#include "NmeBruiser_AiController.h"
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

ANmeBruiser_AiController::ANmeBruiser_AiController(FObjectInitializer const& ObjectInitializer)
{
	//return TEXT("BehaviorTree'/Game/AI/NME/NmeBruiser_BT.NmeBruiser_BT'");
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/AI/NME/NmeBruiser_BT.NmeBruiser_BT'"));
	if (obj.Succeeded())
	{
		bTree = obj.Object;
	}

	BehaviourTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviourComp"));
	Blackboard = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, "BlackboardComp");

	SetupPerceptionSystem();
}

void ANmeBruiser_AiController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(bTree);
	BehaviourTreeComponent->StartTree(*bTree);
}

void ANmeBruiser_AiController::OnPossess(APawn* const InPawn)
{
	Super::OnPossess(InPawn);

	if (Blackboard)
	{
		Blackboard->InitializeBlackboard(*bTree->BlackboardAsset);
	}
}

UBlackboardComponent* ANmeBruiser_AiController::GetBlackboard() const
{
	return Blackboard;
}

void ANmeBruiser_AiController::OnUpdated(TArray<AActor*> const& updatedActors)
{

}

void ANmeBruiser_AiController::OnTargetDetected(AActor* actor, FAIStimulus const stimulus)
{
	if (auto const ch = Cast<AFpsU5CppV1Character>(actor))
	{
		GetBlackboard()->SetValueAsBool(BlackboardKeys::canSeePlayer, stimulus.WasSuccessfullySensed());
	}
}

void ANmeBruiser_AiController::SetupPerceptionSystem()
{
	// Create and initialise sight configuration object

	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
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
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ANmeBruiser_AiController::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*sightConfig);
}
