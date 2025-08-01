// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HealthHUD.h"
#include "Abilities/BaseAbilities.h" 

void UHealthHUD::InitializeFromCharacter(UBaseAbilities* PowerRef)
{
	if (PowerRef && HealthBar)
	{
		float CurrentHealth = PowerRef->GetHealth();
		float MaxHealth = 100.0f;

		UpdateHealthBar(CurrentHealth, MaxHealth);
	}
}

void UHealthHUD::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
	{
		float Percentage = FMath::Clamp(CurrentHealth / MaxHealth, 0.0f, 1.0f);
		HealthBar->SetPercent(Percentage);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HealthBar is not bound in the HUD!"));
	}

}