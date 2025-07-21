#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MetalInteractable.h"
#include "MetallicObject1.generated.h"

UCLASS()
class PROJECTSTEELRUN_API AMetallicObject1 : public AActor, public IMetalInteractable
{
    GENERATED_BODY()

public:
    AMetallicObject1();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Interface implementations
    virtual bool IsMetal_Implementation() const override;
    virtual bool IsTarget_Implementation() const override;

    // Toggles the target state
    virtual void ToggleTarget_Implementation() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

private:
    // Internal target state
    UPROPERTY(VisibleAnywhere, Category = "Metal")
    bool IsTarget = false;
};