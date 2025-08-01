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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Allomancy|Misc", meta = (AllowPrivateAccess = "true"))
	bool CanUseFocusMode = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Allomancy|Force", meta = (AllowPrivateAccess = "true"))
	float ForceMultiplier = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Allomancy|Force", meta = (AllowPrivateAccess = "true"))
	float MinForceMultiplier = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Allomancy|Force", meta = (AllowPrivateAccess = "true"))
	float MaxForceMultiplier = 2500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Allomancy|Force", meta = (AllowPrivateAccess = "true"))
	float ForceAdjustStep = 50.0f;

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
	virtual void ActivateAbility() {}





	void SetCanUseFocusMode(bool CanUse) { CanUseFocusMode = CanUse; }
	bool GetCanUseFocusMode() const { return CanUseFocusMode; }

	void SetForceMultiplier(float SetForce) { ForceMultiplier = SetForce; }
	float GetForceMultiplier() { return ForceMultiplier; }

	void SetMinForceMultiplier(float MinForce) { MinForceMultiplier = MinForce; }
	float GetMinForceMultiplier() const { return MinForceMultiplier; }

	void SetMaxForceMultiplier(float MaxForce) { MaxForceMultiplier = MaxForce; }
	float GetMaxForceMultiplier() const { return MaxForceMultiplier; }

	float GetForceAdjustStep() const { return ForceAdjustStep; }

};