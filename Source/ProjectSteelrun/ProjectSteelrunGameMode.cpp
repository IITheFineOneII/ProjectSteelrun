// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectSteelrunGameMode.h"
#include "Abilities/Helpers/AllomancyComponentFactory.h"

AProjectSteelrunGameMode::AProjectSteelrunGameMode()
{
	// stub
}

void AProjectSteelrunGameMode::BeginPlay()
{
	Super::BeginPlay();
	AllomancyComponentFactory::InitFactory();
	// Log the initialization
	UE_LOG(LogTemp, Warning, TEXT("Project Steelrun AllomancyFactory Initialized"));
}