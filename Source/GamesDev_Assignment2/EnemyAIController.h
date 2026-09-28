#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "ECreatureType.h"
#include "EReactionType.h"
#include "EnemyCharacter.h"
#include "EnemyAIController.generated.h"

UCLASS()
class GAMESDEV_ASSIGNMENT2_API AEnemyAIController : public AAIController
{
    GENERATED_BODY()

public:
    // ***********************
    // Constructor & Overrides
    // ***********************

    AEnemyAIController(const FObjectInitializer& ObjectInitializer);

    virtual void BeginPlay() override;

    // ***********************
    // Behavior / AI Control
    // ***********************

    UFUNCTION()
    void SwitchBehaviorTree(UBehaviorTree* NewTree);

protected:
    // ***********************
    // Engine Overrides
    // ***********************

    UFUNCTION()
    virtual void OnPossess(APawn* InPawn) override;

    // ***********************
    // Perception
    // ***********************

    void SetupPerceptionConfiguration();

    UFUNCTION()
    void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);

    UFUNCTION()
    void OnTargetForgotten(AActor* Actor);

    // ***********************
    // Reaction Logic
    // ***********************

    UFUNCTION()
    int GetFearResponse(ECreatureType Type);

    EReactionType GetReactionType(const int& Response) const;

public:
    // ***********************
    // Get Functions
    // ***********************

    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool HasRangedAttack() const;

    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetMeleeAttackRange() const;

    UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetRangedAttackRange() const;

private:
    // ***********************
    // Ranges
    // ***********************

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    float RangedAttackRange;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    float MeleeAttackRange;

    // ***********************
    // References
    // ***********************

    AEnemyCharacter* ControlledPawn = nullptr;

    class UNavigationSystemV1* NavArea;
    FVector RandomLocation;

    // ***********************
    // Perception Config
    // ***********************

    class UAISenseConfig_Sight* SightConfig;
};