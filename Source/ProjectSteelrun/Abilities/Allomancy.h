#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/Metal.h"
#include "Blueprint/UserWidget.h"
#include "UI/AllomancyHUD.h"
#include "Allomancy.generated.h"

UCLASS(ClassGroup = (Allomancy), meta = (BlueprintSpawnableComponent))
class PROJECTSTEELRUN_API UAllomancy : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Allomancy", meta = (AllowPrivateAccess = "true"))
	EMetal Metal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Allomancy|Resources", meta = (AllowPrivateAccess = "true"))
	float MetalAmount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Allomancy|Resources", meta = (AllowPrivateAccess = "true"))
	float DrainRate = 1.0f;
	TSubclassOf<UUserWidget> AllomancyHUD = nullptr;

public:	
	UAllomancy();
	UAllomancy(EMetal metal);
	virtual void BindInput(UEnhancedInputComponent* InputComponent) {}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	void SetMetalAmount(float NewAmount) { MetalAmount = NewAmount; }
	float GetMetalAmount() const { return MetalAmount; }
	void SetMetal(EMetal NewMetal) { Metal = NewMetal; }
	void DrainResources(float DeltaTime);
	void InitializeHUD();
};