#include "BTTask_IsTargetDead.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_IsTargetDead::UBTTask_IsTargetDead(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Is Target Dead";
}

EBTNodeResult::Type UBTTask_IsTargetDead::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Check if the enemy is dead
	if (AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		if (AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(EnemyAIController->GetPawn()))
		{
			// Check if the target is dead
			if (EnemyCharacter->IsEnemyDead())
			{
				return EBTNodeResult::Failed;
			}
		}
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
