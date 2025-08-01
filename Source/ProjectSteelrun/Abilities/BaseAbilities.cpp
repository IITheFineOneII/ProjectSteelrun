#include "Abilities/BaseAbilities.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/WorldSettings.h"
#include "EnhancedInputComponent.h"
#include "ProjectSteelrunCharacter.h"
#include "Abilities/Allomancy.h"

UBaseAbilities::UBaseAbilities()
{

	PrimaryComponentTick.bCanEverTick = true;


}



void UBaseAbilities::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld())
	{
		OriginalTimeDilation = UGameplayStatics::GetGlobalTimeDilation(GetWorld());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseAbilities: BeginPlay - World is null!"));
	}
	InitializeHUD();
	UUserWidget* HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HealthHUD);
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
		static FName InitFunctionName(TEXT("InitializeFromBaseAbilities"));

		if (UFunction* Function = HUDWidget->FindFunction(InitFunctionName))
		{
			struct FInitializeFromAllomancyParams
			{
				UBaseAbilities* BaseAbilitiesRef;
			};

			FInitializeFromAllomancyParams Params;
			Params.BaseAbilitiesRef = this;

			HUDWidget->ProcessEvent(Function, &Params);
		}
	}
}

void UBaseAbilities::ToggleFocusMode()
{
	AProjectSteelrunCharacter* SteelrunCharacterOwner = Cast<AProjectSteelrunCharacter>(CharacterOwner);
	if (SteelrunCharacterOwner->GetAllomancyComponent()->GetCanUseFocusMode() == true && !IsFocusModeActive)
	{
		ActivateFocusMode();
		
	}
	else if (IsFocusModeActive == true)
	{
		DeactivateFocusMode();
		
	}
}

void UBaseAbilities::ActivateFocusMode()
{
	if (!GetWorld() || IsFocusModeActive) return;

	OriginalTimeDilation = UGameplayStatics::GetGlobalTimeDilation(GetWorld());
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), FocusTimeDilation);

	IsFocusModeActive = true;
}

void UBaseAbilities::DeactivateFocusMode()
{
	if (!GetWorld() || !IsFocusModeActive) return;

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), OriginalTimeDilation);
	IsFocusModeActive = false;
}

void UBaseAbilities::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UBaseAbilities::BindInput(UEnhancedInputComponent* InputComponent)
{
	AProjectSteelrunCharacter* PlayerOwner = Cast<AProjectSteelrunCharacter>(GetOwner());
	if (!InputComponent || !PlayerOwner || !PlayerOwner->ToggleSightAbilityAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseAbilities::BindInput - Missing input/action"));
		return;
	}
	InputComponent->BindAction(
		PlayerOwner->FocusModeAction,
		ETriggerEvent::Started,
		this,
		&UBaseAbilities::ToggleFocusMode
	);
}


void UBaseAbilities::InitializeHUD()
{
	FSoftClassPath WidgetClassPath(TEXT("/Game/UI/WBP_HealthBar.WBP_HealthBar_C"));
	TSubclassOf<UUserWidget> LoadedWidgetClass = Cast<UClass>(WidgetClassPath.TryLoadClass<UUserWidget>());

	if (LoadedWidgetClass)
	{
		HealthHUD = LoadedWidgetClass;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Allomancy HUD Class not found! Please check the path."));
	}
}