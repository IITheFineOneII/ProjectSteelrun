#include "MetallicObject1.h"


// Sets default values
AMetallicObject1::AMetallicObject1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Mesh->SetSimulatePhysics(true);

	IsTarget = false;

}

// Called when the game starts or when spawned
void AMetallicObject1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMetallicObject1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AMetallicObject1::IsMetal_Implementation() const
{
	return true;
}

bool AMetallicObject1::IsTarget_Implementation() const
{
	return IsTarget;
}

void AMetallicObject1::ToggleTarget_Implementation()
{
	IsTarget = !IsTarget;
	UE_LOG(LogTemp, Log, TEXT("Target state set to: %s"), IsTarget ? TEXT("true") : TEXT("false"));

}
