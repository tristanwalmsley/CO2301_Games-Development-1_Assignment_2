#include "BTTask_SetChasing.h"
#include "BehaviorTree/BlackboardComponent.h"

// Task to set the "IsChasing" blackboard key to true, which indicates
// that the enemy should be in chasing mode
EBTNodeResult::Type UBTTask_SetChasing::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Get the blackboard component from the behavior tree component
    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

    // If the blackboard component is valid, set the
    // "IsChasing" key to true and return success
    if (BB)
    {
        BB->SetValueAsBool("IsChasing", true);
        return EBTNodeResult::Succeeded;
    }

    // If the blackboard component is not valid, return failure
    return EBTNodeResult::Failed;
}