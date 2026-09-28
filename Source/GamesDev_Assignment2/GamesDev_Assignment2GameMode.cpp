#include "GamesDev_Assignment2GameMode.h"
#include "Kismet/GameplayStatics.h"

// Constructor
AGamesDev_Assignment2GameMode::AGamesDev_Assignment2GameMode()
{
}

// Called when the game starts
void AGamesDev_Assignment2GameMode::BeginPlay()
{
    Super::BeginPlay();

	// Call StartRound to initialise the first playthrough of the game
    StartRound();
}

// Method to start a new round, which initialises objectives and starts the round timer
void AGamesDev_Assignment2GameMode::StartRound()
{
	// Reset player kills and objectives
    PlayerKills            = 0;
    CurrentObjectiveNumber = 1;

	// Set the initial kills objective based on the first objective number
    KillsObjective = ObjectiveIncrement * CurrentObjectiveNumber;

    // Start round timer
    GetWorld()->GetTimerManager().SetTimer(RoundTimerHandle, this, &AGamesDev_Assignment2GameMode::OnRoundTimeout, RoundTimeSeconds, false);
    
}

// Method called when the round timer expires, which checks if the
// player has met the objectives and ends the round accordingly
void AGamesDev_Assignment2GameMode::OnRoundTimeout()
{
	// Check if player has met the final objective
    if (CurrentObjectiveNumber >= FinalObjectiveNumber)
    {
        EndRound(true); // Player wins
    }
    else
    {
        EndRound(false); // Player loses
    }

}

// Method to end the round, which takes a boolean
// indicating whether the player won or lost
void AGamesDev_Assignment2GameMode::EndRound(bool bPlayerWon)
{
	// Check if the player won or lost and open the
    // appropriate level based on the result
    if (bPlayerWon)
    {
        UGameplayStatics::OpenLevel(this, WinLevelName);

    }
    else
    {
        UGameplayStatics::OpenLevel(this, LoseLevelName);
    }

}

// Method to update the player's kill count, which is called when an enemy
// is killed. It checks if the player has met the current objective &
// updates the objective accordingly, and also checks if the player
// has achieved the final objective early.
bool AGamesDev_Assignment2GameMode::UpdateKills()
{
	// Increment player kills
    PlayerKills ++;

	// Check if player has met the current objective,
    // if so, increment the objective number & update
    // the kills objective for the next objective
    if (PlayerKills >= KillsObjective)
    {
        CurrentObjectiveNumber++;
        KillsObjective = ObjectiveIncrement * CurrentObjectiveNumber;

		// Return true to indicate that the objective was updated
        return true;
    }

	// Check if player has achieved the final objective early and end the round with a win if so
    if (PlayerKills >= KillsObjective && CurrentObjectiveNumber == FinalObjectiveNumber)
    {
        EndRound(true);
    }

	// Return false to indicate that the objective was not updated
    return false;
}

// Method to get the remaining time in the current round,
// which can be called by the UI to display a timer
float AGamesDev_Assignment2GameMode::GetRemainingRoundTime() const
{
	// Check if the world & timer manager are valid before trying to get the remaining time
    if (GetWorld())
    {
        return GetWorld()->GetTimerManager().GetTimerRemaining(RoundTimerHandle);
    }

	// If the world or timer manager is not valid, return 0 to indicate that there is no time remaining
    return 0.f;
}
