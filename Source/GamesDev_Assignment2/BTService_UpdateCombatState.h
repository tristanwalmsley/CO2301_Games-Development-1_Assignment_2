#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UpdateCombatState.generated.h"

UCLASS()
class GAMESDEV_ASSIGNMENT2_API UBTService_UpdateCombatState : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_UpdateCombatState();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};