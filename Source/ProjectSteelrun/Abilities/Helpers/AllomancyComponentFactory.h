#pragma once

#include "CoreMinimal.h"
#include "Abilities/Allomancy.h"
#include "Abilities/SteelAllomancy.h"
#include "Enums/Metal.h"

class PROJECTSTEELRUN_API AllomancyComponentFactory
{
public:
	static TSubclassOf<UAllomancy> GetAllomancyType(EMetal metal);
	static void InitFactory();


private:
	static TMap<EMetal, TSubclassOf<UAllomancy>> MetalToClassMap;
};
