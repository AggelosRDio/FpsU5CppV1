// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "ChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class FPSU5CPPV1_API UChangeSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UChangeSpeed();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	virtual FString GetStaticServiceDescription() const override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = true))
		float speed = 600.0f;
};
