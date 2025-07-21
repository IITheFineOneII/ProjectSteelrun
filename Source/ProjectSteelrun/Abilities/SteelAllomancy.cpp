// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/SteelAllomancy.h"
#include "SteelrunObjectInterface.h"
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
			if (Actor && Actor->GetClass()->ImplementsInterface(USteelrunObjectInterface::StaticClass()))
			{
				bool bIsMetallic = ISteelrunObjectInterface::Execute_IsMetal(Actor);
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
		if (Actor && Actor->GetClass()->ImplementsInterface(USteelrunObjectInterface::StaticClass()))
		{
			if (ISteelrunObjectInterface::Execute_IsTarget(Actor) == true && (Actor && !NearbyMetals.Contains(Actor)))
			{
				ISteelrunObjectInterface::Execute_ToggleTarget(Actor);
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

		if (Object && Object->GetClass()->ImplementsInterface(USteelrunObjectInterface::StaticClass()))
		{
			bool IsTarget = ISteelrunObjectInterface::Execute_IsTarget(Object);
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

	if (!InputComponent || !PlayerOwner || !PlayerOwner->ActivateAbilityAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("SteelAllomancy::BindInput - Missing input/action"));
		return;
	}
	InputComponent->BindAction(
		PlayerOwner->ActivateAbilityAction,
		ETriggerEvent::Started,
		this,
		&USteelAllomancy::ActivateAbility
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

			ISteelrunObjectInterface::Execute_ToggleTarget(HitActor);

		}
	}
}

void USteelAllomancy::ActivateAbility()
{
	if (IsSteelSightActive)
	{
		TArray<AActor*> NearbyMetals = GetNearbyMetalObjects(SteelSightRadius);
		for (AActor* Object : NearbyMetals)
		{
			if (Object && Object->GetClass()->ImplementsInterface(USteelrunObjectInterface::StaticClass()))
			{
				if (ISteelrunObjectInterface::Execute_IsTarget(Object))
				{
					FVector PlayerLocation = CharacterOwner->GetActorLocation();
					FVector ObjectLocation = Object->GetActorLocation();
					FVector Direction = (ObjectLocation - PlayerLocation).GetSafeNormal();

					float Distance = FVector::Distance(PlayerLocation, ObjectLocation);

					float ForceMagnitude = FMath::Clamp(((SteelSightRadius*ForceMultiplier) / Distance), ForceMultiplier, 0.0f);

					UPrimitiveComponent* ObjectComp = Cast<UPrimitiveComponent>(Object->GetRootComponent());
					float ObjectMass = ObjectComp ? ObjectComp->GetMass() : TNumericLimits<float>::Max(); // immovable if no mass

					// Retrieve character mass from CharacterOwner
					AProjectSteelrunCharacter* SteelrunCharacter = Cast<AProjectSteelrunCharacter>(CharacterOwner);
					float PlayerMass = SteelrunCharacter->GetCharacterMass();


					FVector IntendedImpulseOnObject = Direction * ForceMagnitude * (PlayerMass / (PlayerMass + ObjectMass));
					FVector IntendedImpulseOnPlayer = -Direction * ForceMagnitude * (ObjectMass / (PlayerMass + ObjectMass));
					FVector OldVelocity = ObjectComp->GetPhysicsLinearVelocity();
					ObjectComp->AddImpulse(IntendedImpulseOnObject, NAME_None, true);
					FVector NewVelocity = ObjectComp->GetPhysicsLinearVelocity();
					FVector DeltaV = NewVelocity - OldVelocity;

					FVector TotalForce = Direction * ForceMagnitude;
					FVector IntendedDir = IntendedImpulseOnObject.GetSafeNormal();
					float ActualImpulseMagnitude = ObjectMass * FVector::DotProduct(DeltaV, IntendedDir);
					ActualImpulseMagnitude = FMath::Clamp(ActualImpulseMagnitude, 0.0f, IntendedImpulseOnObject.Size());
					float IntendedImpulseMagnitude = IntendedImpulseOnObject.Size();
					float LeftoverFraction = (IntendedImpulseMagnitude > KINDA_SMALL_NUMBER)
						? 1.0f - (ActualImpulseMagnitude / IntendedImpulseMagnitude)
						: 0.0f;
					FVector ExtraImpulseOnPlayer = -Direction * ForceMagnitude * LeftoverFraction * (PlayerMass / (PlayerMass + ObjectMass));
					FVector FinalImpulseOnPlayer = IntendedImpulseOnPlayer + ExtraImpulseOnPlayer;
					ACharacter* Character = Cast<ACharacter>(CharacterOwner);
					if (Character)
					{
						Character->LaunchCharacter(FinalImpulseOnPlayer, true, true);
					}

					UE_LOG(LogTemp, Warning, TEXT("Intended impulse on object: %s | Applied: %f%%"),
						*IntendedImpulseOnObject.ToString(), 100.0f * (1.0f - LeftoverFraction));
					UE_LOG(LogTemp, Warning, TEXT("Final impulse on player: %s"), *FinalImpulseOnPlayer.ToString());
				}
			}
		}
	}
}