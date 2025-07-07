// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectSteelrunCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "MetalInteractable.h"
#include "Engine/OverlapResult.h" 

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AProjectSteelrunCharacter::AProjectSteelrunCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	//Initialize reserves to full
	SteelAmount = 100.f;
	IronAmount = 100.f;
}

void AProjectSteelrunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProjectSteelrunCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AProjectSteelrunCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AProjectSteelrunCharacter::Look);

		EnhancedInputComponent->BindAction(ToggleSteelsightAction, ETriggerEvent::Started, this, &AProjectSteelrunCharacter::ToggleSteelsight);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AProjectSteelrunCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AProjectSteelrunCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AProjectSteelrunCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AProjectSteelrunCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AProjectSteelrunCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AProjectSteelrunCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AProjectSteelrunCharacter::ToggleSteelsight()
{
	bIsSteelSightActive = !bIsSteelSightActive;

	if (bIsSteelSightActive)
	{
		UE_LOG(LogTemp, Warning, TEXT("Steelsight Activated"));

		// TODO: Apply grayscale effect
		// TODO: Start draining steel/iron
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Steelsight Deactivated"));

		// TODO: Remove grayscale effect
		// TODO: Stop draining steel/iron
	}
}

// Setters (clamped between 0 and 100)
void AProjectSteelrunCharacter::SetSteelAmount(float NewAmount)
{
	SteelAmount = FMath::Clamp(NewAmount, 0.f, 100.f);
}

void AProjectSteelrunCharacter::SetIronAmount(float NewAmount)
{
	IronAmount = FMath::Clamp(NewAmount, 0.f, 100.f);
}

float AProjectSteelrunCharacter::GetSteelAmount() const
{
	return SteelAmount;
}

float AProjectSteelrunCharacter::GetIronAmount() const
{
	return IronAmount;
}

void AProjectSteelrunCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (AllomancyHUDClass)
	{
		UUserWidget* TempWidget = CreateWidget<UUserWidget>(GetWorld(), AllomancyHUDClass);
		if (TempWidget)
		{
			TempWidget->AddToViewport();

			// Call the Blueprint event "InitializeFromCharacter" and pass "this"
			FName EventName(TEXT("InitializeFromCharacter"));
			if (TempWidget->IsA(AllomancyHUDClass))
			{
				UFunction* Func = TempWidget->FindFunction(EventName);
				if (Func)
				{
					struct FInitializeParams
					{
						AProjectSteelrunCharacter* CharacterRef;
					};
					FInitializeParams Params;
					Params.CharacterRef = this;
					TempWidget->ProcessEvent(Func, &Params);
				}
			}
		}
	}
}

void AProjectSteelrunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSteelSightActive)
	{
		float DrainRate = 1.0f; // units per second

		// Drain logic
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

		// Clamp to zero
		IronAmount = FMath::Max(IronAmount, 0.0f);
		SteelAmount = FMath::Max(SteelAmount, 0.0f);

		// Optional: Auto-disable when both depleted
		if (IronAmount <= 0 && SteelAmount <= 0)
		{
			bIsSteelSightActive = false;
		}

		float SearchRadius = 1500.0f;
		TArray<AActor*> NearbyMetals = GetNearbyMetalObjects(SearchRadius);

		FVector StartLocation = GetActorLocation() + FVector(0, 0, 50); // Middle of chest

		for (AActor* Metal : NearbyMetals)
		{
			if (!Metal) continue;

			// Try to get the mesh or collision component
			UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Metal->GetComponentByClass(UPrimitiveComponent::StaticClass()));
			FVector TargetLocation = Metal->GetActorLocation(); // Default fallback

			if (PrimComp)
			{
				FVector ClosestPoint;
				PrimComp->GetClosestPointOnCollision(StartLocation, ClosestPoint);
				TargetLocation = ClosestPoint;
			}

			bool bIsTarget = IMetalInteractable::Execute_IsTarget(Metal);
			if (bIsTarget)
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
	}
}

TArray<AActor*> AProjectSteelrunCharacter::GetNearbyMetalObjects(float Radius)
{
	TArray<AActor*> FoundActors;

	FVector Center = GetActorLocation();
	TArray<FOverlapResult> OverlapResults;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

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
				UE_LOG(LogTemp, Warning, TEXT("Target Found"));
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