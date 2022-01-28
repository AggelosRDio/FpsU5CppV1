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

	/* Perception */
	virtual float GetSightRadius() { return SightRadius; }
	virtual	float GetLoseSightRadius() { return LoseSightRadius; }
	virtual	float GetPeripheralVisionAngleDegrees() { return PeripheralVisionAngleDegrees; }
	virtual	float GetMaxAge() { return MaxAge; }
	virtual	float GetAutoSuccessRangeFromLastSeenLocation() { return AutoSuccessRangeFromLastSeenLocation; }
	virtual	bool GetSightDetectEnemies() { return bDetectEnemies; }
	virtual	bool GetSightDetectFriendlies() { return bDetectFriendlies; }
	virtual	bool GetSightDetectNeutrals() { return bDetectNeutrals; }

	virtual	float GetHearingRange() { return HearingRange; }
	virtual bool GetHearingDetectEnemies() { return bHearingDetectEnemies; }
	virtual	bool GetHearingDetectFriendlies() { return bHearingDetectFriendlies; }
	virtual	bool GetHearingDetectNeutrals() { return bHearingDetectNeutrals; }
	
private:
	TCHAR* ObjectToFindBehaviourTree = TEXT("BehaviorTree'/Game/AI/NPC_BT.NPC_BT'");

	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;

	class UAISenseConfig_Sight* sightConfig;

	class UAISenseConfig_Hearing* hearingConfig;

	UFUNCTION()
		void OnUpdated(TArray<AActor*> const& updatedActors);

	void SetupPerceptionSystem();

	/* Perception */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception, meta = (AllowPrivateAccess = true))
		float SightRadius = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception, meta = (AllowPrivateAccess = true))
		float LoseSightRadius = 550.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception, meta = (AllowPrivateAccess = true))
		float PeripheralVisionAngleDegrees = 90.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception, meta = (AllowPrivateAccess = true))
		float MaxAge = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception, meta = (AllowPrivateAccess = true))
		float AutoSuccessRangeFromLastSeenLocation = 900.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception, meta = (AllowPrivateAccess = true))
		bool bDetectEnemies = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception, meta = (AllowPrivateAccess = true))
		bool bDetectFriendlies = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception, meta = (AllowPrivateAccess = true))
		bool bDetectNeutrals = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception, meta = (AllowPrivateAccess = true))
		float HearingRange = 3000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception, meta = (AllowPrivateAccess = true))
		bool bHearingDetectEnemies = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception, meta = (AllowPrivateAccess = true))
		bool bHearingDetectFriendlies = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception, meta = (AllowPrivateAccess = true))
		bool bHearingDetectNeutrals = true;
};
