#include "BTTask_MoveToCharacter.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MoveToCharacter::UBTTask_MoveToCharacter(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Move To Character";
}

// Task to execute the task, which moves the AI character towards the target character
EBTNodeResult::Type UBTTask_MoveToCharacter::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Cast the AI controller to our custom enemy AI controller class, & if successful,
	// call the MoveToActor function to move towards the target character stored in the
	// blackboard, using the melee attack range as the acceptance radius to stop at
	// the appropriate distance for attacking
	if (AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		EnemyAIController->MoveToActor(
			Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetCharacter")),
			(EnemyAIController->GetMeleeAttackRange() / 2.0f)
		);
	}

	return EBTNodeResult::Succeeded;
}
