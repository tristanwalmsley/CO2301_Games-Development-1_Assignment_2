#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.h"
#include "EnemyCharacter_SevarogClose.generated.h"

UCLASS()
class GAMESDEV_ASSIGNMENT2_API AEnemyCharacter_SevarogClose : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter_SevarogClose();

protected:
	virtual void BeginPlay() override;

};