//***********************************
// Task not currently in use
//***********************************

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_AttackTarget.generated.h"

UCLASS()
class GAMESDEV_ASSIGNMENT2_API UBTTask_AttackTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_AttackTarget(const FObjectInitializer& ObjectInitializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
