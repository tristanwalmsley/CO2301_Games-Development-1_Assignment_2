//***********************************
// Task not currently in use
//***********************************

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetChasing.generated.h"

UCLASS()
class GAMESDEV_ASSIGNMENT2_API UBTTask_SetChasing : public UBTTaskNode
{
    GENERATED_BODY()

public:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
