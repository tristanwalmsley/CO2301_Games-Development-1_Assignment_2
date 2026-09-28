#include "Win_GameMode.h"
#include "Kismet/GameplayStatics.h"

// Called when the game starts
void AWin_GameMode::BeginPlay()
{
	// Call the base class BeginPlay to ensure any additional setup is performed
	Super::BeginPlay();

	// Set timer to end the game after the specified duration
	GetWorld()->GetTimerManager().SetTimer(EndLevelTimer, this, &AWin_GameMode::RestartGameLevel, Duration, false);

	// Disable player input during the win screen
	PlayerPawnRef 
		= Cast<AGamesDev_Assignment2Character>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	APlayerController* PlayerController 
		= Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	PlayerPawnRef->DisableInput(PlayerController);

}

// Method to restart the game level, which is called when the end level timer expires
void AWin_GameMode::RestartGameLevel()
{
	// Open the first level to restart the game after the win screen
	UGameplayStatics::OpenLevel(GetWorld(), FName("Level_01"));

}

