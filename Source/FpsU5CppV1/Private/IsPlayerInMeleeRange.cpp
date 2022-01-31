#include "IsPlayerInMeleeRange.h"
#include "NPC_AiController.h"
#include "NPC.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FpsU5CppV1/BlackboardKeys.h"

UIsPlayerInMeleeRange::UIsPlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player in Melee Range");
}

void UIsPlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	// get NPC
	auto const controller = Cast<ANPC_AiController>(OwnerComp.GetAIOwner());
	auto const npc = Cast<ANPC>(controller->GetPawn());

	// Get player character
	auto const playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//npc->GetView

	//write true or false if the player is within melee range
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BlackboardKeys::PlayerIsInMeleeRange, npc->GetDistanceTo(playerCharacter) <= meleeRange);


 }
