#include "Abilities/Helpers/AllomancyComponentFactory.h"

TMap<EMetal, TSubclassOf<UAllomancy>> AllomancyComponentFactory::MetalToClassMap;

void AllomancyComponentFactory::InitFactory()
{
	MetalToClassMap.Add(EMetal::Steel, USteelAllomancy::StaticClass());
	// Add other metals here later
}
TSubclassOf<UAllomancy> AllomancyComponentFactory::GetAllomancyType(EMetal metal)
{
	TSubclassOf<UAllomancy>* Found = MetalToClassMap.Find(metal);
	return Found ? *Found : nullptr;
}
