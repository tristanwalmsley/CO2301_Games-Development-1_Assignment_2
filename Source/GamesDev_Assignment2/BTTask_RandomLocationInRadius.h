#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RandomLocationInRadius.generated.h"

UCLASS()
class GAMESDEV_ASSIGNMENT2_API UBTTask_RandomLocationInRadius : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float searchRadius = 2500.0f; // The radius within which to search for a random location

public:
	UBTTask_RandomLocationInRadius(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
