#include "GamesDev_Assignment2Character.h"
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
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "GamesDev_Assignment2GameMode.h"
#include "GamesDev_Assignment2.h"

AGamesDev_Assignment2Character::AGamesDev_Assignment2Character()
{
	// Collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 250.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Spring Arm for camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	if (GetCharacterMovement())
	{
		// If character movement component is valid, set the walk speed
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

}

// Used to register the character as a stimuli source for AI perception
void AGamesDev_Assignment2Character::SetupStimuliSource()
{
	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));

	if (StimuliSource)
	{
		// Register for sight and hearing senses
		StimuliSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimuliSource->RegisterWithPerceptionSystem();
	}
	else
	{
		// If the component failed to create, log a warning and return
		UE_LOG(LogTemp, Warning, TEXT("Failed to create StimuliSource component"));
		return;
	}

}

void AGamesDev_Assignment2Character::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Unregister from the perception system to clean up properly
	if (StimuliSource)
	{
		StimuliSource->UnregisterFromPerceptionSystem();
	}

	// Call the base class EndPlay to ensure any additional cleanup is performed
	Super::EndPlay(EndPlayReason);
}
void AGamesDev_Assignment2Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(JumpAction,		ETriggerEvent::Started,   this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction,		ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction,		ETriggerEvent::Triggered, this, &AGamesDev_Assignment2Character::Move);
		EnhancedInputComponent->BindAction(LookAction,		ETriggerEvent::Triggered, this, &AGamesDev_Assignment2Character::Look);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AGamesDev_Assignment2Character::Look);
		EnhancedInputComponent->BindAction(FireAction,		ETriggerEvent::Triggered, this, &AGamesDev_Assignment2Character::Fire);
		EnhancedInputComponent->BindAction(EscapeAction,	ETriggerEvent::Triggered, this, &AGamesDev_Assignment2Character::Escape);
	}
}

void AGamesDev_Assignment2Character::BeginPlay()
{
	Super::BeginPlay();

	// Set character health to max at the start of the game
	CurrentHealth = MaxHealth;

	// If the health widget class is set, create the widget and add it to the viewport
	if (HealthWidgetClass)
	{
		HealthWidget = CreateWidget<UUserWidget>(GetWorld(), HealthWidgetClass);

		if (HealthWidget)
		{
			HealthWidget->AddToViewport();
		}
	}

	// If spawn sound 1 is set, play it and then set a timer to play spawn sound 2 after a delay,
	// then after spawn sound 2, then fade into music
	if (SpawnSound1)
	{
		// Play the first spawn sound immediately
		UGameplayStatics::PlaySound2D(this, SpawnSound1);

		// Delay before playing the second sound to avoid overlap
		GetWorldTimerManager().SetTimer(SpawnSoundTimer, [this]()
			{
				// Check if the 2nd spawn sound is valid before trying to play it
				if (SpawnSound2)
				{
					// Play the second spawn sound
					UGameplayStatics::PlaySound2D(this, SpawnSound2);

					// After spawn sounds 1 & 2, fade into music
					GetWorldTimerManager().SetTimer(SpawnSoundTimer, [this]()
						{
							// Pick a random music track from the available options
							USoundBase* music = PickRandomMusic();

							// Check if the music is valid before trying to play it, and play it at a lower volume for ambiance
							if (music)
							{
								UGameplayStatics::PlaySound2D(this, music, MusicVolume);
							}
						}, SpawnSoundTime - 1.0f, false);

				}

			}, SpawnSoundTime, false);

	}

	// Delay input setup
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			// Check if the controller is valid and is a player controller before trying
			// to access the local player and input subsystem
			APlayerController* PC = Cast<APlayerController>(GetController());
			if (!PC)
			{
				return;
			}

			ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
			if (!LocalPlayer)
			{
				return;
			}

			// Get the input subsystem and add the default mapping context
			UEnhancedInputLocalPlayerSubsystem* Subsystem =
				LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

			if (Subsystem && DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	);
}

// Method to handle movement input, which is triggered when the MoveAction is activated
void AGamesDev_Assignment2Character::Move(const FInputActionValue& Value)
{
	// Get the movement input as a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// Call the function to handle movement, passing in the X and Y components of the movement vector
	DoMove(MovementVector.X, MovementVector.Y);
}

// Method to handle the escape action, which quits the game when triggered
void AGamesDev_Assignment2Character::Escape(const FInputActionValue& Value)
{
	// Get the player controller to pass to the QuitGame function
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	// If the player controller is valid, call the QuitGame function to exit the game
	if (PlayerController)
	{
		UKismetSystemLibrary::QuitGame(
			GetWorld(),
			PlayerController,
			EQuitPreference::Quit,
			false
		);
	}
}

// Method to handle look input, which is triggered when the LookAction or MouseLookAction is activated
void AGamesDev_Assignment2Character::Look(const FInputActionValue& Value)
{
	// Get the look input as a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// Call the function to handle looking, passing in the X and Y components
	// of the look vector as yaw and pitch input respectively
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

// Method to handle firing the weapon, which is triggered when the FireAction is activated
void AGamesDev_Assignment2Character::Fire()
{
	// Get the controller to use for determining the direction of fire & applying damage
	// & ensure it's valid before proceeding
	AController* ControllerRef = GetController();
	if (!ControllerRef) return;

	//*****************
	// ANIMATION
	//*****************

	// Play firing animation
	if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
	{
		if (FireMontage)
		{
			Anim->Montage_Play(FireMontage);
		}
	}

	//*****************
	// MUZZLE FLASH
	//*****************

	// Get the location & rotation of the muzzle socket for spawning effects
	FVector MuzzleLocation;
	FRotator MuzzleRotation;

	// Check if the muzzle flash particle system is valid before trying to
	// spawn it, & spawn it at the muzzle location
	if (MuzzleFlash)
	{
		// Get the location and rotation of the muzzle socket
		MuzzleLocation = GetMesh()->GetSocketLocation("Muzzle_01");
		MuzzleRotation = ControllerRef->GetControlRotation();

		// Spawn the muzzle flash effect at the muzzle location and rotation
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, MuzzleLocation, MuzzleRotation);
	}

	//*****************
	// SFX
	//*****************

	// Play firing sound at the character's location
	// Commented out to avoid sound spam, but can be re-enabled if desired
	/*if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}*/

	//*****************
	// RAYCAST
	//*****************

	// Perform a line trace (raycast) from the camera to determine what the player is aiming at
	FVector CameraLocation;
	FRotator CameraRotation;

	// Get the camera location and rotation from the player controller, which will
	// be used as the start point & direction for the line trace
	ControllerRef->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// Calculate the end point of the line trace by extending a vector from
	// the camera in the direction it's facing
	FVector End = CameraLocation + CameraRotation.Vector() * ImpulseStrength;

	// Perform the line trace and store the result in a hit result struct.
	// The trace will check for visibility collisions.
	FHitResult Hit;
	bool bDidHit = GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, End, ECC_Visibility);

	// Check if the bullet trail particle system is valid before trying to spawn it
	if (BulletTrail)
	{
		// Determine the end point for the bullet trail. If we hit something, use the impact point;
		// otherwise, use the end point of the line trace.
		FVector TrailEnd = bDidHit ? Hit.ImpactPoint : End;

		// Spawn the bullet trail effect at the muzzle location, & set its source and target parameters
		UParticleSystemComponent* Trail = UGameplayStatics::
			SpawnEmitterAtLocation(GetWorld(), BulletTrail, MuzzleLocation, MuzzleRotation);

		// Check if the trail was successfully spawned before trying to set parameters
		if (Trail)
		{
			Trail->SetVectorParameter("Source", MuzzleLocation);
			Trail->SetVectorParameter("Target", TrailEnd);
			Trail->bAutoDestroy = true;
		}

	}

	//*****************
	// IMPACT
	//*****************

	// Check if the line trace hit something
	if (bDidHit)
	{
		// Get the actor that was hit by the line trace
		AActor* HitActor = Hit.GetActor();

		if (HitActor)
		{
			// Check if the hit actor is an enemy character.
			if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(HitActor))
			{
				// If it is an enemy, apply damage to it using the ApplyDamage function, passing in the damage amount,
				UGameplayStatics::ApplyDamage(Enemy, DamageAmount, ControllerRef, this, UDamageType::StaticClass());

				// Calculate the direction from the player to the enemy & apply a
				// launch force to the enemy to create a knockback effect in on the
				// X & Y axes, but not the Z axis to avoid launching them into the air
				FVector Direction = FVector(
					Enemy->GetActorLocation().X - GetActorLocation().X,
					Enemy->GetActorLocation().Y - GetActorLocation().Y,
					0.0f
				);

				// Call the LaunchCharacter function on the enemy to apply the knockback force,
				// multiplying the direction by a strength factor
				Enemy->LaunchCharacter(Direction * LaunchDirectionStrengthFactor, true, true);

				// Spawn the appropriate hit effect based on whether we hit a character or the world
				if (HitCharacterEffect)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitCharacterEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
				}

			}
			else
			{
				// If we hit something that isn't an enemy character, spawn the hit world effect at the impact point
				// Check if the hit world effect particle system is valid before trying to spawn it
				if (HitWorldEffect)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitWorldEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
				}

			}
		}

		// Rare chance at special dialogue hit sound
		if (RareHitSound)
		{
			float RandomChance = FMath::FRand();
			if (RandomChance < RareHitsoundChance)
			{
				// Play the rare hit sound at the location of the hit
				UGameplayStatics::PlaySoundAtLocation(this, RareHitSound, Hit.ImpactPoint);
			}

		}

	}

}

// Method to handle movement input, which is called by the Move method after processing input values
void AGamesDev_Assignment2Character::DoMove(float Right, float Forward)
{
	// Get the controller to use for determining movement
	// direction, & ensure it's valid before proceeding
	if (GetController() != nullptr)
	{
		// Get the control rotation from the controller, which represents the
		// direction the player is looking. We will use the yaw component of
		// this rotation to determine the forward & right movement directions.
		// This allows the player to move relative to the direction they are
		// facing, rather than fixed world directions
		const FRotator Rotation		    = GetController()->GetControlRotation();
		const FRotator YawRotation        (0, Rotation.Yaw, 0);
		const FVector  ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector  RightDirection   = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Add movement input in the forward and right directions, scaled by
		// the input values for forward & right movement
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection,   Right  );
	}
}

// Method to handle look input, which is called by the Look method after processing input values
void AGamesDev_Assignment2Character::DoLook(float Yaw, float Pitch)
{
	// Get the controller to use for applying look input, & ensure it's valid before proceeding
	if (GetController() != nullptr)
	{
		// Add controller input for yaw (looking left/right) &
		// pitch (looking up/down) based on the input values
		AddControllerYawInput  (Yaw);
		AddControllerPitchInput(Pitch);
	}
}

// Method to handle jump input, which is triggered when the JumpAction is activated or completed
void AGamesDev_Assignment2Character::DoJumpStart()
{
	// Call the built-in Jump function to make the character jump
	Jump();

	// Play jump sound at the character's location when the jump starts
	if (JumpSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, JumpSound, GetActorLocation());
	}
}

// Method to handle the end of a jump, which is triggered when the JumpAction is completed
void AGamesDev_Assignment2Character::DoJumpEnd()
{
	// Call the built-in StopJumping function to end the jump
	StopJumping();
}

// Method to handle taking damage, which is called when the character receives damage from an enemy or other sources
float AGamesDev_Assignment2Character::TakeDamage(
	float			    damageAmount,    // The amount of damage being applied to the character
	FDamageEvent const& DamageEvent,     // Information about the damage event, such as the type of damage & any other data
	AController*		EventInstigator, // The controller responsible for causing the damage
	AActor*				DamageCauser)    // The actor that directly caused the damage
{
	// Reduce the player's health by the damage amount, but first check if they
	// have any overhealth to absorb the damage. If so, apply damage to that
	// first before applying to current health.
	if (CurrentOverHealth > 0)
	{
		// Apply damage to overhealth first
		CurrentOverHealth -= damageAmount;

		// Check if overhealth is less than or equal to 0, if so, apply remaining damage to current health.
		if (CurrentOverHealth <= 0)
		{
			float RemainingDamage = -CurrentOverHealth; // Invert overhealth to get remaining damage

			CurrentOverHealth = 0;				 // Set overhealth to 0
			CurrentHealth	 -= RemainingDamage; // Apply remaining damage to current health
		}

	}
	else
	{
		// No overhealth, apply damage directly to current health
		CurrentHealth -= damageAmount;
	}

	// Set the bWasHit flag to true to trigger hit reactions
	bWasHit = true;

	// Play hit sound at the character's location when hit, but only
	// if the character isn't dead yet to avoid sound spam on death
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}

	// Play hit reaction animation using a montage
	if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
	{
		if (HitMontage)
		{
			Anim->Montage_Play(HitMontage);
		}
	}

	// After applying damage & playing hit reactions, check if the character's
	// health has dropped to 0 or below, & if so, handle death
	IsCharacterDead();

	// Return the damage amount that was applied
	return damageAmount;
}

// Method to check if the character is dead, which is called after taking
// damage to determine if death handling should be triggered
bool AGamesDev_Assignment2Character::IsCharacterDead()
{
	// Check if current health is less than or equal to 0, if so,
	// set the bIsDead flag to true & call the HandleDeath function
	// to play death reactions & handle respawning later
	if (CurrentHealth <= 0.0f)
	{
		bIsDead = true;
		HandleDeath();

		// Return true to indicate that the character is dead
		return true;
	}
	else
	{
		// Character is not dead, return false
		return false;
	}
}

// Method to handle the character's death, which is called when health drops to 0 or below
void AGamesDev_Assignment2Character::HandleDeath()
{
	// Play death animation using a montage
	if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
	{
		if (DeathMontage)
		{
			Anim->Montage_Play(DeathMontage);
		}
	}

	// Play death sound at the character's location when they die
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	// Disable player input & movement
	GetCharacterMovement()->DisableMovement();

	// Get all enemy characters in the world and call a
	// function on them to react to the player's death
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), FoundEnemies);
	for (AActor* Actor : FoundEnemies)
	{
		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Actor);

		if (Enemy)
		{
			Enemy->ReactToPlayerDeath();
		}

	}

	// Switch camera to a random enemies periodically after player death
	// to show their reactions, & set a timer to switch every 1 second
	GetWorldTimerManager().SetTimer(CameraSwitchTimer, this, &AGamesDev_Assignment2Character::SwitchCameraToRandomEnemy, 1.0f, true);
	
	// Play enemy dialogue on player death
	if (DeathDialogueSound)
	{
		UGameplayStatics::PlaySound2D(this, DeathDialogueSound);
	}

	// Set a timer to end the game after a delay, which will also clear the camera
	// switch timer & reset the view target back to the player character
	FTimerHandle RespawnTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, [this]()
		{
			// Get the player controller and set the view target back to the
			// player character to clear the camera switch timer
			APlayerController* PC = Cast<APlayerController>(GetController());
			if (PC)
			{
				GetWorldTimerManager().ClearTimer(CameraSwitchTimer);
				PC->SetViewTargetWithBlend(this, 0.5f);

				// After resetting the camera, notify the game mode that the
				// round has ended with a loss for the player
				if (AGamesDev_Assignment2GameMode* GM 
					= Cast<AGamesDev_Assignment2GameMode>(UGameplayStatics::GetGameMode(GetWorld())))
				{
					GM->EndRound(false);
				}

			}

		}, RespawnTimer, false);

}

// Method to switch the camera to a random enemy character,
// which is called periodically after the player's death
void AGamesDev_Assignment2Character::SwitchCameraToRandomEnemy()
{
	// Get all enemy characters in the world
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), FoundEnemies);

	// If there are no enemies found, return early to avoid errors
	if (FoundEnemies.Num() == 0) return;

	// Pick a random enemy from the list of found enemies
	int32 Index = FMath::RandRange(0, FoundEnemies.Num() - 1);
	AActor* NewTarget = FoundEnemies[Index];

	// Check if the new target is a valid enemy character before trying to switch the camera
	if (AEnemyCharacter* EnemyTarget = Cast<AEnemyCharacter>(NewTarget))
	{
		// Get the player controller & set the view target
		// to the new enemy target to switch the camera
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			PC->SetViewTarget(EnemyTarget);
			CurrentCameraTarget = EnemyTarget;
		}
	}
}

// Method to pick a random music track from the available options, which
// is called after the spawn sounds to start background music
USoundBase* AGamesDev_Assignment2Character::PickRandomMusic()
{
	TArray<USoundBase*> MusicOptions = { Music1, Music2, Music3, Music4, Music5 };
	int32				RandomIndex  = FMath::RandRange(0, MusicOptions.Num() - 1);
	return MusicOptions[RandomIndex];
}

// Method to check if the player has reached the goal, which is called in the
// Tick function to determine if the round should end with a victory
bool AGamesDev_Assignment2Character::HasPlayerReachedGoal() const
{
	// Check if the player's X location is greater than or equal to the goal location
	return GetActorLocation().X >= LocationXGoal;
}

// Method called every frame
void AGamesDev_Assignment2Character::Tick(float DeltaTime)
{
	// Call the base class Tick function to ensure any additional functionality is preserved
	Super::Tick(DeltaTime);

	// Check if the player has reached the goal location
	if (HasPlayerReachedGoal())
	{
		// Notify game mode of victory
		if (AGamesDev_Assignment2GameMode* GM 
			= Cast<AGamesDev_Assignment2GameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			GM->EndRound(true);
		}
	}
}
