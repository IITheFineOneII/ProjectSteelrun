// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Allomancy.h"
#include "SteelAllomancy.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Allomancy), meta = (BlueprintSpawnableComponent))
class PROJECTSTEELRUN_API USteelAllomancy : public UAllomancy
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Allomancy|Steel", meta = (AllowPrivateAccess = "true"))
	bool IsSteelSightActive = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Allomancy|Steel", meta = (AllowPrivateAccess = "true"))
	float SteelSightRadius = 1500.0f;


	AActor* CharacterOwner = GetOwner();
public:
	USteelAllomancy();
	~USteelAllomancy();

public:
	void GenerateSteellines();
	void ToggleSteelsight();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BindInput(UEnhancedInputComponent* InputComponent) override;


	TArray<AActor*> GetNearbyMetalObjects(float Radius);


};
