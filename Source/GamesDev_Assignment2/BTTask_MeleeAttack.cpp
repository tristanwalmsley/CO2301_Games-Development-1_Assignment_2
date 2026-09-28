#include "BTTask_MeleeAttack.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "GamesDev_Assignment2Character.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Melee Attack";
}

// Task to perform a melee attack on the player character.
// This task is executed when the enemy is in range to attack
// the player & has decided to perform a melee attack based
// on its reaction type.
EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the enemy AI controller from the behavior tree component, & ensure it's valid before proceeding
	if (AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		// Get the enemy character from the AI controller's pawn, & ensure it's valid before proceeding
		if (AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(EnemyAIController->GetPawn()))
		{
			// Get the player character from the blackboard, which should have been set by the AI
			// controller when it detected the player, & ensure it's valid before proceeding
			if (AGamesDev_Assignment2Character* PlayerCharacter = Cast<AGamesDev_Assignment2Character>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetCharacter")))
			{
				// Call the MeleeAttackPlayer method on the enemy character, passing
				// in the player character as a parameter to perform the attack
				EnemyCharacter->MeleeAttackPlayer(PlayerCharacter);

				return EBTNodeResult::Succeeded;
			}

		}

	}

	// If any of the casts failed or we couldn't get the necessary references,
	// return Failed to indicate the task did not complete successfully
	return EBTNodeResult::Failed;
}
