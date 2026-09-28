//***********************************
// Task not currently in use
//***********************************

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_IsTargetDead.generated.h"

UCLASS()
class GAMESDEV_ASSIGNMENT2_API UBTTask_IsTargetDead : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_IsTargetDead(const FObjectInitializer& ObjectInitializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
