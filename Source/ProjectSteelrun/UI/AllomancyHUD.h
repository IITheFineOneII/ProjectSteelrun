#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Enums/Metal.h"
#include "AllomancyHUD.generated.h"

class UAllomancy;


UCLASS()
class PROJECTSTEELRUN_API UAllomancyHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeFromCharacter(UAllomancy* PowerRef);

	UFUNCTION()
	void UpdateMetalBar(float CurrentMetal, float MaxMetal);

	void SetMetalBarColor(EMetal MetalType);
protected:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* AllomancyBar;
};
