#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECreatureType : uint8
{
	CT_CHARACTER UMETA(DisplayName = "Character"),
	CT_SEVAROG   UMETA(DisplayName = "Sevarog")
};