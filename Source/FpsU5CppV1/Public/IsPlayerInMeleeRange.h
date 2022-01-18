#pragma once

#include"BehaviorTree/Services/BTService_BlackboardBase.h"
#include "IsPlayerInMeleeRange.generated.h"

UCLASS()
class FPSU5CPPV1_API UIsPlayerInMeleeRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UIsPlayerInMeleeRange();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = true))
		float meleeRange = 25.0f;
};