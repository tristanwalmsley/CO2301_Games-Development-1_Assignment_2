#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.h"
#include "EnemyCharacter_Sevarog_Regular.generated.h"

UCLASS()
class GAMESDEV_ASSIGNMENT2_API AEnemyCharacter_Sevarog_Regular : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter_Sevarog_Regular();

protected:
	virtual void BeginPlay() override;

};