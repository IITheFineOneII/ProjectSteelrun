#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SteelrunObjectInterface.h"
#include "SteelrunObject1.generated.h"

UCLASS()
class PROJECTSTEELRUN_API ASteelrunObject1 : public AActor, public ISteelrunObjectInterface
{
    GENERATED_BODY()

public:
    ASteelrunObject1();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Metal")
    bool IsMetal = true;

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