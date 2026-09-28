#include "BTService_UpdateTarget.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdateTarget::UBTService_UpdateTarget()
{
	NodeName = "Update Target";
	Interval = 0.2f;
}

// This service ticks every 0.2 seconds & updates the "TargetKnown"
// blackboard key based on whether the "TargetCharacter" key has a
// valid actor reference
void UBTService_UpdateTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// Get the AI controller that owns this behavior tree, & ensure it's valid before proceeding
	AEnemyAIController* AI = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (!AI) return;

	// Get the blackboard component from the behavior tree component, & ensure it's valid before proceeding
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return;

	// Get the target actor from the blackboard using the "TargetCharacter" key, & check if it's valid
	AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetCharacter"));

	// Set the "TargetKnown" blackboard key to true if the target is valid, or false if it's not
	// valid. This allows the behavior tree to react to whether the AI has a target or not.
	BB->SetValueAsBool("TargetKnown", Target != nullptr);
}