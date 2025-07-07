#include "SteelAllomancyComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h" 
#include "ProjectSteelrunCharacter.h"
#include "MetalInteractable.h"

USteelAllomancyComponent::USteelAllomancyComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USteelAllomancyComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<AProjectSteelrunCharacter>(GetOwner());
}

void USteelAllomancyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsSteelsightActive)
	{
		DrainResources(DeltaTime);

		if (SteelAmount <= 0 && IronAmount <= 0)
		{
			bIsSteelsightActive = false;
			return;
		}

		TArray<AActor*> Metals = FindNearbyMetals(ScanRadius);
		VisualizeMetalTargets(Metals);
	}
}

void USteelAllomancyComponent::ToggleSteelsight()
{
	bIsSteelsightActive = !bIsSteelsightActive;
	UE_LOG(LogTemp, Warning, TEXT("Steelsight %s"), bIsSteelsightActive ? TEXT("Activated") : TEXT("Deactivated"));
}

bool USteelAllomancyComponent::IsSteelsightActive() const
{
	return bIsSteelsightActive;
}

void USteelAllomancyComponent::DrainResources(float DeltaTime)
{
	if (IronAmount > 0 && SteelAmount > 0)
	{
		IronAmount -= DrainRate * DeltaTime;
		SteelAmount -= DrainRate * DeltaTime;
	}
	else if (IronAmount <= 0 && SteelAmount > 0)
	{
		SteelAmount -= DrainRate * 2 * DeltaTime;
	}
	else if (SteelAmount <= 0 && IronAmount > 0)
	{
		IronAmount -= DrainRate * 2 * DeltaTime;
	}

	IronAmount = FMath::Max(IronAmount, 0.f);
	SteelAmount = FMath::Max(SteelAmount, 0.f);
}

TArray<AActor*> USteelAllomancyComponent::FindNearbyMetals(float Radius) const
{
	TArray<AActor*> FoundActors;

	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerCharacter is null in FindNearbyMetals"));
		return FoundActors;
	}

	FVector Center = OwnerCharacter->GetActorLocation();
	TArray<FOverlapResult> OverlapResults;

	FCollisionQueryParams Params(TEXT("SteelScan"), false);
	Params.AddIgnoredActor(OwnerCharacter);

	bool bHit = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		Center,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldDynamic),
		FCollisionShape::MakeSphere(Radius),
		Params
	);

	if (!bHit)
	{
		return FoundActors;
	}

	for (const FOverlapResult& Result : OverlapResults)
	{
		AActor* Actor = Result.GetActor();

		if (!Actor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Overlap result had a null Actor"));
			continue;
		}

		if (!Actor->GetClass()->ImplementsInterface(UMetalInteractable::StaticClass()))
		{
			UE_LOG(LogTemp, Verbose, TEXT("Actor %s does not implement UMetalInteractable"), *Actor->GetName());
			continue;
		}

		if (IMetalInteractable::Execute_IsMetal(Actor))
		{
			FoundActors.Add(Actor);
		}
	}

	return FoundActors;
}

void USteelAllomancyComponent::VisualizeMetalTargets(const TArray<AActor*>& Metals) const
{
	if (!OwnerCharacter) return;

	FVector StartLocation = OwnerCharacter->GetActorLocation() + FVector(0, 0, 50);

	for (AActor* Metal : Metals)
	{
		if (!Metal) continue;

		UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Metal->GetComponentByClass(UPrimitiveComponent::StaticClass()));
		FVector TargetLocation = Metal->GetActorLocation();

		if (PrimComp)
		{
			FVector ClosestPoint;
			PrimComp->GetClosestPointOnCollision(StartLocation, ClosestPoint);
			TargetLocation = ClosestPoint;
		}

		const bool bIsTarget = IMetalInteractable::Execute_IsTarget(Metal);
		const FColor LineColor = bIsTarget ? FColor::Orange : FColor::Blue;

		DrawDebugLine(GetWorld(), StartLocation, TargetLocation, LineColor, false, 0.0f, 0, 2.0f);
	}
}

void USteelAllomancyComponent::SetSteelAmount(float NewAmount)
{
	SteelAmount = FMath::Clamp(NewAmount, 0.f, 100.f);
}

void USteelAllomancyComponent::SetIronAmount(float NewAmount)
{
	IronAmount = FMath::Clamp(NewAmount, 0.f, 100.f);
}

float USteelAllomancyComponent::GetSteelAmount() const { return SteelAmount; }
float USteelAllomancyComponent::GetIronAmount() const { return IronAmount; }