#include "BTTask_RangedAttack.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "GamesDev_Assignment2Character.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_RangedAttack::UBTTask_RangedAttack(const FObjectInitializer& ObjectInitializer)
{
    NodeName = "Ranged Attack";
}

// Task to perform a ranged attack, which calls the ShootProjectile method
// on the enemy character & returns success if the attack was performed
EBTNodeResult::Type UBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Get the AI controller that owns this behavior tree task,
    // & ensure it's valid before proceeding
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    // Get the enemy character that the AI controller is controlling,
    // & ensure it's valid before proceeding
    AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(AIController->GetPawn());
    if (!EnemyCharacter) return EBTNodeResult::Failed;

    // Get the blackboard component from the behavior tree owner
    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

    // Get the target character from the blackboard, which is stored
    // as an object reference, so we need to cast it to an AActor pointer,
    // & ensure it's valid before proceeding
    AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetCharacter"));
    if (!Target) return EBTNodeResult::Failed;

    // Call the ShootProjectile method on the enemy character to perform the attack
    EnemyCharacter->ShootProjectile();
    return EBTNodeResult::Succeeded;
}
