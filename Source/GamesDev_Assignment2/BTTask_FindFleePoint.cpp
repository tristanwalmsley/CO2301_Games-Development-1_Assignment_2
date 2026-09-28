#include "BTTask_FindFleePoint.h"
#include "EnemyAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_FindFleePoint::UBTTask_FindFleePoint(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Find Flee Point";

}

// Task to find a point to flee to, which is away from the target character and within a certain
// distance from the NPC. The found point is stored in the blackboard for use by other tasks.
EBTNodeResult::Type UBTTask_FindFleePoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI controller and ensure it's valid before proceeding
	if (AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		// Get the NPC pawn controlled by the AI controller and ensure it's valid before proceeding
		if (APawn* npc = EnemyAIController->GetPawn())
		{
			// Get the navigation system to use for finding a flee point, and ensure it's valid before proceeding
			if (UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				// Get the current transform of the NPC, the location of the target character from the
				// blackboard, & calculate the direction to flee in
				FTransform currentTransform = npc->GetTransform();
				FVector    fleePoint = OwnerComp.GetBlackboardComponent()->GetValueAsVector("TargetCharacter");
				FRotator   fleeDirection = UKismetMathLibrary::FindLookAtRotation(currentTransform.GetLocation(), fleePoint);

				fleeDirection.Yaw += 180.0f; // Invert the direction to flee away from the target
				fleeDirection.Pitch = 0.0f; // Keep the pitch level
				fleeDirection.Roll = 0.0f; // Keep the roll level

				// Update the current transform's rotation to face the flee direction
				currentTransform.SetRotation(fleeDirection.Quaternion());

				// Get the target character from the blackboard and ensure it's valid before proceeding
				AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetCharacter"));

				// If the target is not valid, we cannot find a flee point, so return
				// failed to indicate the task was unsuccessful
				if (!Target) return EBTNodeResult::Failed;

				// Get the location of the target character, which we will use to
				// calculate the flee direction & find a point away from it
				FVector targetLocation = Target->GetActorLocation();

				// Calculate the flee direction by getting the vector from the
				// target to the NPC, normalizing it, and then multiplying by
				// the desired flee distance
				FVector npcLocation = npc->GetActorLocation();
				FVector fleeDir = (npcLocation - targetLocation).GetSafeNormal();
				FVector targetPoint = npcLocation + (fleeDir * FleeDistance);

				// Use the navigation system to find a random point within a certain
				// radius of the target point that is on the nav mesh
				FNavLocation newLocation;
				bool bFoundFlee = false;

				// We will try to find a flee point, adjusting the target point
				// slightly each time if we fail to find a valid point
				if (navSystem->GetRandomPointInNavigableRadius(targetPoint, FleeDistance, newLocation))
				{
					bFoundFlee = true;
				}

				// If we found a valid flee point, store it in the blackboard &
				// set a flag to indicate that we are fleeing, then return
				// succeeded to indicate the task was successful
				if (bFoundFlee)
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), newLocation.Location);
					OwnerComp.GetBlackboardComponent()->SetValueAsBool("bIsFleeing", true);

					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}

			} // End if (UNavigationSystemV1...

		} // End if (APawn...

	} // End if (AEnemyAIController...

	// If we failed to find a flee point for any reason, return
	// failed to indicate the task was unsuccessful
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}
