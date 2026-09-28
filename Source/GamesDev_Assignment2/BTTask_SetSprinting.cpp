#include "BTTask_SetSprinting.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"

UBTTask_SetSprinting::UBTTask_SetSprinting(const FObjectInitializer& ObjectInitializer)
{
    NodeName = "Set Sprinting";
}

// Task to set the sprinting state of the enemy character, which
// is used to determine whether the character should be moving
// at an increased speed
EBTNodeResult::Type UBTTask_SetSprinting::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Cast the AI controller & enemy character to access the SetSprinting method
    if (AEnemyAIController* AI = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
    {
        // Get the sprinting state from the blackboard & call the SetSprinting method on the enemy character
        if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AI->GetPawn()))
        {
            Enemy->SetSprinting(bSprint);
            return EBTNodeResult::Succeeded;
        }
    }

    // If we fail to cast the AI controller or enemy character, return
    // failed to indicate the task did not complete successfully
    return EBTNodeResult::Failed;
}