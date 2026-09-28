#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GamesDev_Assignment2GameMode.generated.h"

UCLASS(abstract)
class AGamesDev_Assignment2GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGamesDev_Assignment2GameMode();

	// ***********************
	// Engine Overrides
	// ***********************

	virtual void BeginPlay() override;

	// ***********************
	// Gameplay State
	// ***********************

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PlayerKills;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int KillsObjective;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ObjectiveIncrement = 7;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int FinalObjectiveNumber = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentObjectiveNumber;

	// ***********************
	// Round Settings
	// ***********************

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RoundTimeSeconds = 150;

	FTimerHandle RoundTimerHandle;

	// ***********************
	// Levels
	// ***********************

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Levels")
	FName WinLevelName = "Win_Level";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Levels")
	FName LoseLevelName = "Lose_Level";

	// ***********************
	// Game Functions
	// ***********************

	void StartRound	   ();
	void OnRoundTimeout();
	void EndRound	   (bool bPlayerWon);
	bool UpdateKills   ();

	int  GetCurrentObjectiveNumber() { return CurrentObjectiveNumber; }

	UFUNCTION(BlueprintCallable, Category = "Timer")
	float GetRemainingRoundTime() const;
};