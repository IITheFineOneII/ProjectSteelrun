
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SteelAllomancyComponent.generated.h"

class AProjectSteelrunCharacter;

UCLASS(ClassGroup = (Allomancy), meta = (BlueprintSpawnableComponent))
class PROJECTSTEELRUN_API USteelAllomancyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USteelAllomancyComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Core ability interface
	void ToggleSteelsight();
	bool IsSteelsightActive() const;

	// Resource setters
	void SetSteelAmount(float NewAmount);
	void SetIronAmount(float NewAmount);

	// Resource getters
	float GetSteelAmount() const;
	float GetIronAmount() const;

protected:
	// Scans world for interactables
	TArray<AActor*> FindNearbyMetals(float Radius) const;

	// Line drawing
	void VisualizeMetalTargets(const TArray<AActor*>& Metals) const;

	// Resource drain
	void DrainResources(float DeltaTime);

private:
	bool bIsSteelsightActive = false;

	UPROPERTY(EditDefaultsOnly, Category = "Allomancy|Resources")
	float SteelAmount = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Allomancy|Resources")
	float IronAmount = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Allomancy|Resources")
	float DrainRate = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Allomancy|Steelsight")
	float ScanRadius = 1500.f;

	UPROPERTY()
	AProjectSteelrunCharacter* OwnerCharacter;
};
