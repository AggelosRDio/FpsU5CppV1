// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "NPC_AiController.generated.h"

/**
 * 
 */
UCLASS()
class FPSU5CPPV1_API ANPC_AiController : public AAIController
{
	GENERATED_BODY()

public:
	ANPC_AiController(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* const InPawn) override;

	UBlackboardComponent* GetBlackboard() const;
private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = true))
		class UBehaviorTreeComponent* BehaviourTreeComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = true))
		UBehaviorTree* bTree;

	UBlackboardComponent* Blackboard;

	class UAISenseConfig_Sight* sightConfig;

	UFUNCTION()
		void OnUpdated(TArray<AActor*> const& updatedActors);

	UFUNCTION()
		void OnTargetDetected(AActor* actor, FAIStimulus const stimulus);

	void SetupPerceptionSystem();
};
