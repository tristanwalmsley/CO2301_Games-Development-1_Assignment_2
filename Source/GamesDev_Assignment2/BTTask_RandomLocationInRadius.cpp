#include "BTTask_RandomLocationInRadius.h"
#include "EnemyAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_RandomLocationInRadius::UBTTask_RandomLocationInRadius(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Random Location In Radius";
}

// Task to find a random location within a specified radius & set it in the
// blackboard for the AI to move towards. This task is used to make the AI
// move to random locations within a certain area, which can be useful for
// patrolling or fleeing from the player
EBTNodeResult::Type UBTTask_RandomLocationInRadius::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI controller & ensure it's valid before proceeding
	if (AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		// Get the controlled pawn (NPC) & ensure it's valid before proceeding
		if (APawn* npc = EnemyAIController->GetPawn())
		{
			// Get the current location of the NPC, which will be used as the center
			// point for finding a random location within the specified radius
			FVector currentLocation = npc->GetActorLocation();

			// Get the navigation system, which is used to find random points in the world
			// that are navigable for the AI. Ensure it's valid before proceeding
			if (UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				// Variable to store the random location that will
				// be found by the navigation system.
				FNavLocation randomLocation;

				// The GetRandomPointInNavigableRadius function is used to find a
				// random point in the world that is within the specified radius of
				// the current location & is navigable for the AI. If a valid
				// point is found, it will be stored in the randomLocation variable.
				if (navSystem->GetRandomPointInNavigableRadius(currentLocation, searchRadius, randomLocation))
				{
					// Variable to store the new location that will be set in
					// the blackboard for the AI to move towards.
					FNavLocation newLocation;

					// The GetRandomReachablePointInRadius function is used to find
					// a random point in the world that is within the specified
					// radius of the current location, is navigable for the AI,
					// & is reachable from the current location. This ensures that
					// the AI can actually move to the location that is found.
					// If a valid point is found, it will be stored in the
					// newLocation variable.
					if (navSystem->GetRandomReachablePointInRadius(currentLocation, searchRadius, newLocation))
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), newLocation.Location);
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

						return EBTNodeResult::Succeeded;
					}

				} // End if (navSystem...

			} // End if (UNavigationSystemV1...

		} // End if (APawn...

	} // End if (AEnemyAIController...

	// If we fail to find a valid random location for any reason,
	// finish the task with a failure result
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}
