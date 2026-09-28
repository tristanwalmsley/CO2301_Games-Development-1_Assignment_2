#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.h"
#include "EnemyCharacter_SevarogFar.generated.h"

UCLASS()
class GAMESDEV_ASSIGNMENT2_API AEnemyCharacter_SevarogFar : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter_SevarogFar();

protected:
	virtual void BeginPlay() override;

};