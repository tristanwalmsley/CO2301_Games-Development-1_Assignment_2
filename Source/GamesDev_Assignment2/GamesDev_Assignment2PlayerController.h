#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamesDev_Assignment2PlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;

UCLASS(abstract)
class AGamesDev_Assignment2PlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	// Input Mapping Contexts
	UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	// Gameplay initialization
	virtual void BeginPlay() override;

	// Input mapping context setup
	virtual void SetupInputComponent() override;

};
