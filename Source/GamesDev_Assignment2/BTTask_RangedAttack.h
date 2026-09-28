#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RangedAttack.generated.h"

UCLASS()
class GAMESDEV_ASSIGNMENT2_API UBTTask_RangedAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_RangedAttack(const FObjectInitializer& ObjectInitializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
