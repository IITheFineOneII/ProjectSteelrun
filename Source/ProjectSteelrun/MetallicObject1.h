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
	// Sets default values for this actor's properties
	AMetallicObject1();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual bool IsMetal_Implementation() const;

	virtual bool IsTarget_Implementation() const;


	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;
};
