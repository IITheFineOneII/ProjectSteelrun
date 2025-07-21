// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/SteelAllomancy.h"
#include "MetalInteractable.h"
#include "Engine/OverlapResult.h"
#include "ProjectSteelrunCharacter.h"
#include "EnhancedInputComponent.h"

USteelAllomancy::USteelAllomancy()
{
	PrimaryComponentTick.bCanEverTick = true;
}

USteelAllomancy::~USteelAllomancy()
{
}

void USteelAllomancy::ToggleSteelsight()
{
	IsSteelSightActive = !IsSteelSightActive;

	if (IsSteelSightActive)
	{
		UE_LOG(LogTemp, Warning, TEXT("Steelsight Activated"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Steelsight Deactivated"));
	}
}



TArray<AActor*> USteelAllomancy::GetNearbyMetalObjects(float Radius)
{
	TArray<AActor*> FoundActors;

	FVector Center = CharacterOwner->GetActorLocation();
	TArray<FOverlapResult> OverlapResults;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(CharacterOwner);

	bool bHasHit = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		Center,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldDynamic),
		FCollisionShape::MakeSphere(Radius),
		Params
	);

	if (bHasHit)
	{
		for (const FOverlapResult& Result : OverlapResults)
		{
			AActor* Actor = Result.GetActor();
			if (Actor && Actor->GetClass()->ImplementsInterface(UMetalInteractable::StaticClass()))
			{
				bool bIsMetallic = IMetalInteractable::Execute_IsMetal(Actor);
				if (bIsMetallic)
				{
					FoundActors.Add(Actor);
				}
			}
		}
	}

	return FoundActors;
}

void USteelAllomancy::GenerateSteellines()
{
	TArray<AActor*> NearbyMetals = GetNearbyMetalObjects(SteelSightRadius);

	for (AActor* Actor : PreviousMetalObjects)
	{
		if (Actor && Actor->GetClass()->ImplementsInterface(UMetalInteractable::StaticClass()))
		{
			if (IMetalInteractable::Execute_IsTarget(Actor) == true && (Actor && !NearbyMetals.Contains(Actor)))
			{
				IMetalInteractable::Execute_ToggleTarget(Actor);
			}
		}
	}

	FVector StartLocation = CharacterOwner->GetActorLocation() + FVector(0, 0, 40); // Middle of chest

	for (AActor* Object : NearbyMetals)
	{
		if (!Object) continue;
		UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Object->GetComponentByClass(UPrimitiveComponent::StaticClass()));
		FVector TargetLocation = Object->GetActorLocation(); // Default fallback if we can't split up object into points

		if (PrimComp)
		{
			FVector ClosestPoint;
			PrimComp->GetClosestPointOnCollision(StartLocation, ClosestPoint);
			TargetLocation = ClosestPoint; // Get the closest point on the metal object to the start location
		}

		if (Object && Object->GetClass()->ImplementsInterface(UMetalInteractable::StaticClass()))
		{
			bool IsTarget = IMetalInteractable::Execute_IsTarget(Object);
			if (IsTarget)
			{
				DrawDebugLine(
					GetWorld(),
					StartLocation,
					TargetLocation,
					FColor::Orange,
					false,
					0.0f,
					0,
					2.0f
				);
			}
			else
			{
				DrawDebugLine(
					GetWorld(),
					StartLocation,
					TargetLocation,
					FColor::Blue,
					false,
					0.0f,
					0,
					2.0f
				);
			}
		}
		else
		{
			DrawDebugLine(
				GetWorld(),
				StartLocation,
				TargetLocation,
				FColor::Red,
				false,
				0.0f,
				0,
				2.0f
			);
		}
	}
	PreviousMetalObjects = NearbyMetals; // Update the previous metal objects for the next tick
}

void USteelAllomancy::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsSteelSightActive)
	{
		DrainResources(DeltaTime);
		if (GetMetalAmount() <= 0)
		{
			IsSteelSightActive = false;
			UE_LOG(LogTemp, Warning, TEXT("Steelsight Deactivated due to resource depletion"));
		}
		GenerateSteellines();
	}
}

void USteelAllomancy::BindInput(UEnhancedInputComponent* InputComponent)
{
	AProjectSteelrunCharacter* PlayerOwner = Cast<AProjectSteelrunCharacter>(GetOwner());
	if (!InputComponent || !PlayerOwner || !PlayerOwner->ToggleSightAbilityAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("SteelAllomancy::BindInput - Missing input/action"));
		return;
	}
	InputComponent->BindAction(
		PlayerOwner->ToggleSightAbilityAction,
		ETriggerEvent::Started,
		this,
		&USteelAllomancy::ToggleSteelsight
	);

	if (!InputComponent || !PlayerOwner || !PlayerOwner->SelectTargetAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("SteelAllomancy::BindInput - Missing input/action"));
		return;
	}
	InputComponent->BindAction(
		PlayerOwner->SelectTargetAction,
		ETriggerEvent::Started,
		this,
		&USteelAllomancy::ToggleTargeting
	);
}

void USteelAllomancy::ToggleTargeting()
{
	FVector Start = CharacterOwner->GetActorLocation();
	FVector ForwardVector = CharacterOwner->GetActorForwardVector();
	FVector End = Start + ForwardVector * SteelSightRadius;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(CharacterOwner);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		AActor* HitActor = HitResult.GetActor();
		if (!HitActor) return;

		TArray<AActor*> AllowedActors = GetNearbyMetalObjects(SteelSightRadius);
		if (AllowedActors.Contains(HitActor))
		{

			IMetalInteractable::Execute_ToggleTarget(HitActor);

		}
	}
}