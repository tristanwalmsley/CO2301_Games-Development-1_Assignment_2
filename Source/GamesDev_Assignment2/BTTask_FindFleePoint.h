#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindFleePoint.generated.h"

UCLASS()
class GAMESDEV_ASSIGNMENT2_API UBTTask_FindFleePoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float FleeDistance = 3000.0f; // The distance the enemy will try to flee to when fleeing

public:
	UBTTask_FindFleePoint(const FObjectInitializer& ObjectInitializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
