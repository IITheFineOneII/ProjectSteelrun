#pragma once

#include "CoreMinimal.h"
#include "Metal.generated.h"


UENUM(BlueprintType)
enum class EMetal : uint8
{
	None = 0 UMETA(Hidden),
	Iron = 1 UMETA(DisplayName = "Iron"),
	Steel = 2 UMETA(DisplayName = "Steel"),
	Pewter = 3 UMETA(DisplayName = "Pewter"),
	Tin = 4 UMETA(DisplayName = "Tin"),
	Copper = 5 UMETA(DisplayName = "Copper"),
	Bronze = 6 UMETA(DisplayName = "Bronze"),
	Brass = 7 UMETA(DisplayName = "Brass"),
	Zinc = 8 UMETA(DisplayName = "Zinc")
};
