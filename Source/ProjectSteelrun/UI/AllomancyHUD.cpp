#include "UI/AllomancyHUD.h"  
#include "Abilities/Allomancy.h" 

void UAllomancyHUD::InitializeFromCharacter(UAllomancy* PowerRef)  
{  
	if (PowerRef && AllomancyBar)  
	{  
		float CurrentMetal = PowerRef->GetMetalAmount();  
		float MaxMetal = 100.0f;  

		UpdateMetalBar(CurrentMetal, MaxMetal);  
	}  
}  

void UAllomancyHUD::UpdateMetalBar(float CurrentMetal, float MaxMetal)  
{  
	if (AllomancyBar)  
	{  
		float Percentage = FMath::Clamp(CurrentMetal / MaxMetal, 0.0f, 1.0f);  
		AllomancyBar->SetPercent(Percentage);  
	}  
	else  
	{  
		UE_LOG(LogTemp, Warning, TEXT("AllomancyBar is not bound in the HUD!"));  
	}  
}

void UAllomancyHUD::SetMetalBarColor(EMetal MetalType)  
{  
	if (AllomancyBar)  
	{  
		FLinearColor Color;  
		switch (MetalType)  
		{  
			case EMetal::Iron:  
				Color = FLinearColor(0.282f, 0.298f, 0.314f);
				break;  
			case EMetal::Steel:  
				Color = FLinearColor(0.69f, 0.77f, 0.87f);
				break;  
			case EMetal::Tin:  
				Color = FLinearColor(0.745f, 0.823f, 0.784f);
				break;  
			case EMetal::Pewter:  
				Color = FLinearColor(0.412f, 0.376f, 0.329f);
				break;  
			case EMetal::Zinc:
				Color = FLinearColor(0.557f, 0.706f, 0.804f);
				break;
			case EMetal::Brass:
				Color = FLinearColor(0.71f, 0.475f, 0.22f);
				break;
			case EMetal::Copper:
				Color = FLinearColor(0.722f, 0.451f, 0.2f);
				break;
			case EMetal::Bronze:
				Color = FLinearColor(0.804f, 0.498f, 0.196f);
				break;
			default:  
				Color = FLinearColor::Black;  
				break;  
		}  
		AllomancyBar->SetFillColorAndOpacity(Color);  
	}  
}