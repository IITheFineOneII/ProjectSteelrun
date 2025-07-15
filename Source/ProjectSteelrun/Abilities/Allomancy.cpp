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
	Super::BeginPlay();
	InitializeHUD();
	UUserWidget* HUDWidget = CreateWidget<UUserWidget>(GetWorld(), AllomancyHUD);
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
		static FName InitFunctionName(TEXT("InitializeFromAllomancy"));

		if (UFunction* Function = HUDWidget->FindFunction(InitFunctionName))
		{
			struct FInitializeFromAllomancyParams
			{
				UAllomancy* AllomancyRef;
			};

			FInitializeFromAllomancyParams Params;
			Params.AllomancyRef = this;

			HUDWidget->ProcessEvent(Function, &Params);
		}
	}
	SetMetalAmount(100.0f);
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

void UAllomancy::InitializeHUD()
{
	FSoftClassPath WidgetClassPath(TEXT("/Game/UI/WBP_AllomancyBar.WBP_AllomancyBar_C"));
	TSubclassOf<UUserWidget> LoadedWidgetClass = Cast<UClass>(WidgetClassPath.TryLoadClass<UUserWidget>());

	if (LoadedWidgetClass)
	{
		AllomancyHUD = LoadedWidgetClass;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Allomancy HUD Class not found! Please check the path."));
	}
}