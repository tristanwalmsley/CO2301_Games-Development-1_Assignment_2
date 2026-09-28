// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GamesDev_Assignment2Character.h"

// Constructor
AEnemyAIController::AEnemyAIController(const FObjectInitializer& ObjectInitializer)
{
    // Setup the perception configuration for the AI controller,
    // which includes configuring sight & other senses
    SetupPerceptionConfiguration();
}

// Sets default values
void AEnemyAIController::BeginPlay()
{
	// Call the base class BeginPlay to ensure any additional setup is performed
    Super::BeginPlay();

}

// Method to check if the enemy has a ranged attack, which is used
// in the behavior tree to determine which attack to use
bool AEnemyAIController::HasRangedAttack() const
{
    return false;
}

// Method to get the range of the enemy's melee attack
float AEnemyAIController::GetMeleeAttackRange() const
{
    return MeleeAttackRange;
}

// Method to get the range of the enemy's ranged attack
float AEnemyAIController::GetRangedAttackRange() const
{
    return RangedAttackRange;
}

// Method called when the AI controller possesses a pawn, which initialises
// the behavior tree & blackboard for the controlled enemy character
void AEnemyAIController::OnPossess(APawn* InPawn)
{
	// Call the base class OnPossess to ensure any additional setup is performed
    Super::OnPossess(InPawn);

	// Check if the possessed pawn is an enemy character, & if so, initialise
    // the behavior tree & blackboard for that character. If the pawn is
    // not an enemy character, log an error message.
    if (AEnemyCharacter* npc = Cast<AEnemyCharacter>(InPawn))
    {
		// If the cast is successful, set the controlled pawn to the possessed enemy character
        ControlledPawn = npc;

		// Initialise attack ranges from the controlled pawn's properties
        RangedAttackRange = ControlledPawn->RangedAttackRange;
        MeleeAttackRange  = ControlledPawn->MeleeAttackRange;

		// If the controlled pawn has a valid behavior tree,
        // initialise the blackboard & run the behavior tree
        if (UBehaviorTree* behaviorTree = ControlledPawn->GetBehaviorTree())
        {
            UBlackboardComponent* BB;
			UseBlackboard(behaviorTree->BlackboardAsset, BB);
            Blackboard = BB;
            RunBehaviorTree(behaviorTree);
		}

    }

}

// Method to set up the perception configuration for the AI controller, which includes
// configuring sight & other senses, as well as setting up the appropriate callbacks
// for when targets are detected or forgotten
void AEnemyAIController::SetupPerceptionConfiguration()
{
	// Create the perception component for the AI controller, which
    // will be used to detect targets in the environment
    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

	// Check if the perception component was created successfully,
    // & if not, return early to avoid further issues
    if (!IsValid(PerceptionComponent))
    {
        return;
    }

	// Create the sight configuration for the AI controller, which will
    // define how the AI detects targets using sight, including parameters
	// such as sight radius, peripheral vision angle, & detection by affiliation
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

    if (SightConfig)
    {
		SetPerceptionComponent(*PerceptionComponent);

		SightConfig->SightRadius = 1000.0f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 100.0f; // Add a buffer to the lose sight radius to prevent
                                                                          // flickering when targets are near the edge of sight
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;                // 180 degrees total vision cone
		SightConfig->SetMaxAge(2.0f);                                     // Time in seconds that a stimulus is considered
                                                                          // valid after being sensed
		SightConfig->DetectionByAffiliation.bDetectEnemies    = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals   = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

		// Set the dominant sense for the perception component to sight, so
        // that the AI will primarily use sight to detect targets
		PerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());

		// Add the sight configuration to the perception component,
        // which will enable the AI to use sight to detect targets
        PerceptionComponent->OnTargetPerceptionUpdated.  AddDynamic(this, &AEnemyAIController::OnTargetDetected);
		PerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &AEnemyAIController::OnTargetForgotten);
		PerceptionComponent->ConfigureSense(*SightConfig);

    }

}

// Callback function called when a target is detected by the
// AI's perception system, which updates the blackboard
void AEnemyAIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	// Attempt to cast the detected actor to the player character class,
    // which is the only valid target for this AI. If the cast
    // fails, return early.
    AGamesDev_Assignment2Character* AsCharacter = Cast<AGamesDev_Assignment2Character>(Actor);

	// If the cast fails, it means the detected actor is not a
    // valid target for this AI, so we return early to avoid
    // further processing
    if (!AsCharacter) return;

	// Check if the stimulus is currently valid. If the stimulus is not valid,
    // return early to avoid updating the blackboard with an invalid target.
    if (!Stimulus.WasSuccessfullySensed())
    {
        return;
    }

	// If we have a valid target, we calculate the fear response
    // value based on the creature type of the target character,
	// & then determine the appropriate reaction type based
    // on that response value
    int ResponseValue = GetFearResponse(AsCharacter->GetCreatureType());
    EReactionType Reaction = GetReactionType(ResponseValue);

	// Update the blackboard with the new reaction value,
    // target character, & target known status.
    if (Blackboard)
    {
        int currentReaction = Blackboard->GetValueAsInt("ReactionValue");

        if (currentReaction != (int)Reaction)
        {
            Blackboard->SetValueAsInt("ReactionValue", (int)Reaction);
        }

        Blackboard->SetValueAsObject("TargetCharacter", AsCharacter);
        Blackboard->SetValueAsBool  ("TargetKnown",     true       );

    }

}

// Method called when a target is forgotten by the AI's
// perception system, which updates the blackboard
void AEnemyAIController::OnTargetForgotten(AActor* Actor)
{
	// Check if the forgotten actor is the same as the current target
    // character stored in the blackboard. If it is, update the
    // blackboard to indicate that the target is no longer known
    // & store the last known location of the target.
	AActor* KnownTarget = Cast<AActor>(this->GetBlackboardComponent()->GetValueAsObject("TargetCharacter"));

	// Only update the blackboard if the forgotten actor
    // is the same as the current target character,
	if (KnownTarget == Actor)
	{
		this->GetBlackboardComponent()->SetValueAsBool  ("TargetKnown",    false                    );
		this->GetBlackboardComponent()->SetValueAsVector("TargetLocation", Actor->GetActorLocation());
	}
}

// Method to calculate the fear response value based on the creature type of the target
// character, which is used to determine the appropriate reaction type for the AI
int AEnemyAIController::GetFearResponse(ECreatureType Type)
{
	// The fear response is calculated based on the creature type of the target character
    switch (ControlledPawn->GetCreatureType())
    {
		// If the controlled pawn is a character, it has no fear response
        // to other characters, but has a strong fear response to Sevarog
    case ECreatureType::CT_CHARACTER:
        switch (Type)
        {
        case ECreatureType::CT_CHARACTER:
            return 0;
        case ECreatureType::CT_SEVAROG:
			return 4;
        }
        break;
		// If the controlled pawn is a Sevarog, it has a strong fear response
        // to characters, but no fear response to other Sevarogs
    case ECreatureType::CT_SEVAROG:
        switch (Type)
        {
        case ECreatureType::CT_CHARACTER:
			return 4;
        case ECreatureType::CT_SEVAROG:
			return 0;
        }
        break;
    }

	return 0;
}

// Method to determine the appropriate reaction type for the AI based on the fear response value,
// as well as the current health of the controlled pawn. If the health is low, the AI will
// prioritize fleeing regardless of the fear response value, to ensure that it tries to survive
// as long as possible.
EReactionType AEnemyAIController::GetReactionType(const int& Response) const
{
	// If the controlled pawn's health is low, we prioritize fleeing regardless of the fear response value,
	// to ensure that the AI tries to survive as long as possible. If the health is not low, we determine
	// the reaction type based on the fear response value, using the attack and flee thresholds
    // defined in the controlled pawn.
    if (ControlledPawn->IsHealthLow())
    {
        return EReactionType::RT_FLEE;
    }
    else if (Response >= ControlledPawn->GetAttackThreshold())
    {
        return EReactionType::RT_ATTACK;
    }
    else if (Response <= ControlledPawn->GetFleeThreshold())
    {
        return EReactionType::RT_FLEE;
    }

	// If the response value does not meet either threshold, we
    // return the default reaction type of ignore
    return EReactionType::RT_IGNORE;
}

// Method to switch the behavior tree of the AI controller, which can be called
// from the behavior tree itself to change the AI's behavior dynamically based
// on certain conditions
void AEnemyAIController::SwitchBehaviorTree(UBehaviorTree* NewTree)
{
	// If the new behavior tree is valid, run it to switch the AI's behavior. This allows
	// the behavior tree to dynamically change the AI's behavior based on certain conditions,
	// such as switching to a shooting behavior tree when the AI decides to attack, or switching
	// to a fleeing behavior tree when the AI decides to flee. If the new behavior tree is not
    // valid, we do nothing to avoid errors.
    if (NewTree)
    {
        RunBehaviorTree(NewTree);

    }

}
