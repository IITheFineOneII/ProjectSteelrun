#include "Allomancy.h"
#include "Blueprint/UserWidget.h"
#include "UI/AllomancyHUD.h"

// Sets default values for this component's properties
UAllomancy::UAllomancy()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

UAllomancy::UAllomancy(EMetal metal)
{
	PrimaryComponentTick.bCanEverTick = true;
	Metal = metal;
}


// Called when the game starts
void UAllomancy::BeginPlay()
{
	UAllomancyHUD* HUD = CreateWidget<UAllomancyHUD>(GetWorld(), AllomancyHUDClass);
	Super::BeginPlay();
	if (AllomancyHUDClass)
	{
		HUD->AddToViewport();
		HUD->InitializeFromCharacter(this); // Passing self to fill the mana bar
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("Allomancy HUD Class not set! Please set it in the component properties."));
	}
	
}


void UAllomancy::DrainResources(float DeltaTime)
{
	if (MetalAmount > 0)
	{
		MetalAmount -= DrainRate * DeltaTime;
		if (MetalAmount < 0)
		{
			MetalAmount = 0;
		}
	}
}

