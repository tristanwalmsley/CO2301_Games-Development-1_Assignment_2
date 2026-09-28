#include "BTService_UpdateCombatState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "GameFramework/Actor.h"

UBTService_UpdateCombatState::UBTService_UpdateCombatState()
{
	NodeName = "Update Combat State";

	Interval = 0.2f;
	RandomDeviation = 0.05f; // Add a small random deviation to the interval to
	// prevent all AI from updating on the same tick
}

// This service checks the distance to the target & updates the Blackboard
// with whether the target is in melee or ranged attack range, as well as
// whether the AI can currently attack or shoot based on cooldowns
void UBTService_UpdateCombatState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// Call the base class TickNode to ensure any additional setup is performed
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Get the AI controller & ensure it's valid before proceeding
	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (!AIController) return;

	// Get the enemy character controlled by the AI & ensure it's valid before proceeding
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AIController->GetPawn());
	if (!Enemy) return;

	// Get the Blackboard component & ensure it's valid before proceeding
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return;

	// Get the target actor from the Blackboard & ensure it's valid before proceeding
	// If the target is not valid, we can assume we are not in combat & set all
	// combat-related Blackboard values to false to prevent the AI from trying to
	// attack or shoot without a target, & then return early to avoid errors
	AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject("TargetCharacter"));
	if (!Target)
	{
		Blackboard->SetValueAsBool("InMeleeRange", false);
		Blackboard->SetValueAsBool("InRangedRange", false);
		Blackboard->SetValueAsBool("CanAttack", false);
		Blackboard->SetValueAsBool("CanShoot", false);

		return;
	}

	// Calculate distance to target
	float Distance = FVector::Dist(Enemy->GetActorLocation(), Target->GetActorLocation());

	// Get the melee & ranged attack ranges from the enemy character,
	// which may be used to determine whether the target is in
	// range for either type of attack
	float MeleeRange = AIController->GetMeleeAttackRange();
	float RangedRange = AIController->GetRangedAttackRange();

	// Update Blackboard values based on distance to target
	Blackboard->SetValueAsBool("InMeleeRange", Distance <= MeleeRange);
	Blackboard->SetValueAsBool("InRangedRange", Distance <= RangedRange);

	// Update Blackboard values for whether the AI can attack or
	// shoot based on the enemy character's current state, which
	// may be determined by cooldowns or other factors in the
	// character's logic that are updated in its Tick function
	Blackboard->SetValueAsBool("CanAttack", Enemy->bCanAttack);
	Blackboard->SetValueAsBool("CanShoot", Enemy->bCanShoot);
}