#include "BTTask_AttackTarget.h"
#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GamesDev_Assignment2Character.h"

UBTTask_AttackTarget::UBTTask_AttackTarget(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Attack Validity Check";
}

// Task to check if the target is valid & attack if possible, returning success if the attack was initiated
EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI controller and enemy character references, & check if they are valid
	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (!AIController) return EBTNodeResult::Failed;

	// Get the enemy character that this AI controller is controlling, & check if it's valid
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AIController->GetPawn());
	if (!Enemy) return EBTNodeResult::Failed;

	// Get the blackboard component from the behavior tree component, & check if it's valid
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;

	// Get the player character reference from the blackboard, which is
	// stored as an object and needs to be cast to the correct type, &
	// check if it's valid before proceeding
	AGamesDev_Assignment2Character* Player =
		Cast<AGamesDev_Assignment2Character>(Blackboard->GetValueAsObject("TargetCharacter"));

	// If the player reference is not valid, return failed
	// to indicate that we couldn't attack
	if (!Player) return EBTNodeResult::Failed;

	// If we have a valid player reference, call the MeleeAttackPlayer
	// method on the enemy character to initiate the attack
	Enemy->MeleeAttackPlayer(Player);

	return EBTNodeResult::Succeeded;
}
