#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blueprint/UserWidget.h"
#include "UI/AllomancyHUD.h"
#include "BaseAbilities.generated.h"


UCLASS( ClassGroup=(Ability), meta=(BlueprintSpawnableComponent) )
class PROJECTSTEELRUN_API UBaseAbilities : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBaseAbilities();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ToggleFocusMode();
	void ActivateFocusMode();
	void DeactivateFocusMode();
	void BindInput(UEnhancedInputComponent* InputComponent);
	void InitializeHUD();
private: 
	AActor* CharacterOwner = GetOwner();
	bool IsFocusModeActive = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float Health = 100.0f;
	float OriginalTimeDilation = 1.0f;
	float FocusTimeDilation = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> HealthHUD = nullptr;

public:
	void SetHealth(int NewHealth) { Health = NewHealth; }
	int GetHealth() const { return Health; }
};
