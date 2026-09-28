#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MoveToCharacter.generated.h"

UCLASS()
class GAMESDEV_ASSIGNMENT2_API UBTTask_MoveToCharacter : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_MoveToCharacter(const FObjectInitializer& ObjectInitializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
