#include "GamesDev_Assignment2PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "GamesDev_Assignment2.h"

// Called when the game starts or when spawned
void AGamesDev_Assignment2PlayerController::BeginPlay()
{
	// Call the base class BeginPlay to ensure any additional setup is performed
	Super::BeginPlay();

}

void AGamesDev_Assignment2PlayerController::SetupInputComponent()
{
	// Call the base class SetupInputComponent to ensure any additional setup is performed
	Super::SetupInputComponent();

	// Only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

		}

	}

}
