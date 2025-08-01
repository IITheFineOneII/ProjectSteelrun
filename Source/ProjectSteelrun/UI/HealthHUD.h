// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetBlueprint.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HealthHUD.generated.h"

class UBaseAbilities;

UCLASS()
class PROJECTSTEELRUN_API UHealthHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	void InitializeFromCharacter(UBaseAbilities* PowerRef);

	UFUNCTION()
	void UpdateHealthBar(float CurrentHealth, float MaxHealth);

protected:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
};
