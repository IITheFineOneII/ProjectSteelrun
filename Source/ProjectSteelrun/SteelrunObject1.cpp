#include "SteelrunObject1.h"


// Sets default values
ASteelrunObject1::ASteelrunObject1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Mesh->SetSimulatePhysics(true);

	IsTarget = false;

}

// Called when the game starts or when spawned
void ASteelrunObject1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASteelrunObject1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ASteelrunObject1::IsMetal_Implementation() const
{
	return IsMetal;
}

bool ASteelrunObject1::IsTarget_Implementation() const
{
	return IsTarget;
}


void ASteelrunObject1::ToggleTarget_Implementation()
{
	IsTarget = !IsTarget;
	UE_LOG(LogTemp, Log, TEXT("Target state set to: %s"), IsTarget ? TEXT("true") : TEXT("false"));

}
