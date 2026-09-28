#pragma once

#include "CoreMinimal.h"

// Enum for the different types of reactions an enemy can have
UENUM(BlueprintType)
enum class EReactionType : uint8
{
	RT_FLEE   UMETA(DisplayName = "Flee"),
	RT_IGNORE UMETA(DisplayName = "Ignore"),
	RT_ATTACK UMETA(DisplayName = "Attack")
};