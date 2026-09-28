#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GamesDev_Assignment2Character.h"
#include "Engine/World.h"
#include "EnemyProjectile.h"
#include "Math/UnrealMathUtility.h"
#include "Camera/CameraComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/CapsuleComponent.h"
#include "GamesDev_Assignment2GameMode.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// initialise health
	CurrentHealth = MaxHealth;

	// Register AI Perception Stimuli Source
	SetupStimuliSource();

	// Spectator Camera
	EnemyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("EnemyCamera"));
	EnemyCamera->SetupAttachment(RootComponent);

	// Point Light
	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetupAttachment(RootComponent);

	// Point Light 2
	PointLight2 = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight2"));
	PointLight2->SetupAttachment(RootComponent);

	// Spot Light
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(RootComponent);

	// Collision settings
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	GetCapsuleComponent()->SetCapsuleSize(70.f, 150.f);

	// Possession settings
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	// Call the base class BeginPlay to ensure any additional setup is performed
	Super::BeginPlay();

	// Setup
	EnemyAIController = Cast<AEnemyAIController>(GetController());

	SetSprinting(false);

	AnimInstance = GetMesh()->GetAnimInstance();

	// Health regeneration timer
	GetWorld()->GetTimerManager().SetTimer(HealthRegenTimer, this, &AEnemyCharacter::RegenerateHealth, HealthRegenInterval, true);

	// Spectator Camera
	EnemyCamera->SetRelativeLocation(EnemyCamera_Location);
	EnemyCamera->SetRelativeRotation(EnemyCamera_Rotation);

	// Point Light
	PointLight->SetRelativeLocation	(PointLight_Location);
	PointLight->SetRelativeRotation	(PointLight_Rotation);
	PointLight->SetLightColor		(PointLight_LightColor);
	PointLight->SetIntensity		(PointLight_Intensity);
	PointLight->SetAttenuationRadius(PointLight_AttenuationRadius);
	PointLight->SetCastShadows		(PointLight_CastShadows);
	PointLight->SetSourceRadius		(PointLight_SourceRadius);

	// Point Light 2
	PointLight2->SetRelativeLocation (PointLight2_Location);
	PointLight2->SetRelativeRotation (PointLight2_Rotation);
	PointLight2->SetLightColor		 (PointLight2_LightColor);
	PointLight2->SetIntensity		 (PointLight2_Intensity);
	PointLight2->SetAttenuationRadius(PointLight2_AttenuationRadius);
	PointLight2->SetCastShadows		 (PointLight2_CastShadows);
	PointLight2->SetSourceRadius	 (PointLight2_SourceRadius);
	PointLight2->SetSoftSourceRadius (PointLight2_SoftSourceRadius);
	PointLight2->SetSourceLength	 (PointLight2_SourceLength);

	// Spot Light
	SpotLight->SetRelativeLocation (SpotLight_Location);
	SpotLight->SetRelativeRotation (SpotLight_Rotation);
	SpotLight->SetLightColor	   (SpotLight_LightColor);
	SpotLight->SetIntensity		   (SpotLight_Intensity);
	SpotLight->SetAttenuationRadius(SpotLight_AttenuationRadius);
	SpotLight->SetCastShadows      (SpotLight_CastShadows);
	SpotLight->SetSourceRadius	   (SpotLight_SourceRadius);
	SpotLight->SetInnerConeAngle   (SpotLight_InnerConeAngle);
	SpotLight->SetOuterConeAngle   (SpotLight_OuterConeAngle);

}

// Method to set the enemy's movement speed based on whether they are sprinting or not
void AEnemyCharacter::SetSprinting(bool bSprint)
{
	// Check if the character movement component exists before trying to set the speed
	if (GetCharacterMovement())
	{
		// Set the max walk speed to the sprint speed if sprinting,
		// otherwise set it to the normal walk speed
		GetCharacterMovement()->MaxWalkSpeed = bSprint ? SprintSpeed : WalkSpeed;
	}
}

// Method to regenerate the enemy's health over time,
// which is called by a timer every few seconds
void AEnemyCharacter::RegenerateHealth()
{
	// If the enemy is dead, don't regenerate health
	if (IsDead) return;

	// Regenerate health by adding the regen rate to the current health,
	// but clamp it to the max health to avoid overhealing
	CurrentHealth = FMath::Min(CurrentHealth + HealthRegenRate, MaxHealth);

	// Update behavior after regenerating health to potentially
	// switch behaviour tree or state if health is high enough
	UpdateBehavior();
}

// Method to react to the player's death, which is
// called by the player character when they die
void AEnemyCharacter::ReactToPlayerDeath()
{
	// Play death reaction animation montage
	if (PlayerDeathReactionAnimationMontage)
	{
		AnimInstance->Montage_Play(PlayerDeathReactionAnimationMontage);
	}

	// After reacting to the player's death, start a timer to
	// reset the enemy's attack ability after a cooldown
	GetWorld()->GetTimerManager().SetTimer(
		AttackCooldownTimer,
		this,
		&AEnemyCharacter::ResetAttack,
		AttackCooldown,
		false
	);

}

// Method to shoot a projectile towards the player, which is called
// by the behavior tree when the enemy is in range to shoot
void AEnemyCharacter::ShootProjectile()
{
	// Check if the enemy can shoot before trying to spawn a projectile
	if (!bCanShoot) return;

	// Check if the projectile class is set before trying to spawn it
	if (!ProjectileClass) return;

	// Calculate the spawn location & rotation for the projectile
	// based on the enemy's location and forward vector
	FVector  MuzzleLocation = GetActorLocation()
		+ GetActorForwardVector() * 200.f + FVector(0, 0, 0.f);
	FRotator MuzzleRotation = GetActorRotation();

	// Set spawn parameters for the projectile, including the owner & instigator
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	// Spawn the projectile in the world using the specified
	// class, location, rotation, & spawn parameters
	AEnemyProjectile* Projectile = GetWorld()->SpawnActor<AEnemyProjectile>(
		ProjectileClass,
		MuzzleLocation,
		MuzzleRotation,
		SpawnParams
	);

	// If the projectile was successfully spawned, initialise its properties
	if (Projectile)
	{
		Projectile->InitProjectile(
			ProjectileDamage,
			ProjectileInitialSpeed,
			ProjectileMaxSpeed,
			ProjectileLifeSpan
		);
	}

	// Start shoot cooldown
	bCanShoot = false;

	GetWorld()->GetTimerManager().SetTimer(
		FireTimer,
		this,
		&AEnemyCharacter::ResetShoot,
		FireRate,
		false
	);
}

// Method to setup the AI Perception Stimuli Source component, which
// allows the enemy to be detected by AI perception systems
void AEnemyCharacter::SetupStimuliSource()
{
	// Create the AI Perception Stimuli Source component &
	// register it for sight and hearing senses
	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));

	// Check if the stimuli source component was successfully
	// created before trying to register it
	if (StimuliSource)
	{
		StimuliSource->RegisterForSense(UAISense_Sight::StaticClass());
		StimuliSource->RegisterForSense(UAISense_Hearing::StaticClass());
		StimuliSource->RegisterWithPerceptionSystem();
	}
	else
	{
		// If the stimuli source component couldn't be
		// created, return early to avoid errors
		return;
	}

}

// Method to unregister the AI Perception Stimuli Source component when the enemy is 
// destroyed, which is called by the engine when the enemy is removed from the world
void AEnemyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Unregister from the perception system to clean up properly when the enemy is destroyed
	if (StimuliSource)
	{
		StimuliSource->UnregisterFromPerceptionSystem();
	}

	// Call the base class EndPlay to ensure any additional cleanup is performed
	Super::EndPlay(EndPlayReason);
}

// Method to perform a melee attack on the player character,
// which is called by the behavior tree when the enemy
// is in range to melee attack
void AEnemyCharacter::MeleeAttackPlayer(AGamesDev_Assignment2Character* PlayerCharacter)
{
	// Check if the enemy can attack & if the player character
	// is not already dead before trying to attack
	if (!bCanAttack || PlayerCharacter->bIsDead) return;

	// Set bCanAttack to false to prevent multiple attacks during the cooldown period
	bCanAttack = false;

	// Play animation
	if (AnimInstance && EnemyAttackAnimation)
	{
		AnimInstance->Montage_Play(EnemyAttackAnimation);
	}

	// Deal damage to the player character
	DealDamage(PlayerCharacter);

	// Sounds
	if (AttackSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetActorLocation(), 1.0f, Pitch, 0.0f, SoundAttenuation);
	}

	// Start attack cooldown timer to reset bCanAttack
	// after the specified cooldown duration
	if (PlayerCharacter->bIsDead)
	{
		// If the player is dead after the attack,
		// don't start the cooldown timer
		return;
	}
	else
	{
		// If the player is still alive after the attack,
		// start the cooldown timer to reset bCanAttack
		GetWorld()->GetTimerManager().SetTimer(
			AttackCooldownTimer,
			this,
			&AEnemyCharacter::ResetAttack,
			AttackCooldown,
			false
		);
	}
}

// Method to reset the enemy's ability to attack, which
// is called by a timer after the attack cooldown expires
void AEnemyCharacter::ResetAttack()
{
	bCanAttack = true;

}

// Method to reset the enemy's ability to shoot, which
// is called by a timer after the shoot cooldown expires
void AEnemyCharacter::ResetShoot()
{
	bCanShoot = true;
	
}

// Method to apply damage to the player character, which is called during a melee attack
void AEnemyCharacter::DealDamage(AGamesDev_Assignment2Character* PlayerCharacter)
{
	// Check if the player character is valid before trying to apply damage
	if (PlayerCharacter)
	{
		// Apply damage to the player character using the ApplyDamage
		// function, passing in the damage amount,
		UGameplayStatics::ApplyDamage(
			PlayerCharacter,
			DamageAmount,
			EnemyAIController,
			this,
			nullptr);
	}
}

// Method to teleport the enemy away after death, which is
// called by a timer after the death animation finishes
void AEnemyCharacter::TeleportAway()
{
	// Teleport enemy 10000 directly down out of the level to effectively
	// remove them from the game world without destroying the actor,
	// this method is much more efficient than destroying & respawning
	// the actor for the respawn mechanic
	FVector NewLocation = GetActorLocation();
	NewLocation.Z -= 10000.0f;
	SetActorLocation(NewLocation);

	// Disable collision to prevent the enemy from interacting with the world while teleported away
	SetActorEnableCollision(false);

	// Start timer to respawn the enemy back up after a delay
	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &AEnemyCharacter::Respawn, RespawnDuration, false);
}

// Method to handle taking damage, which is called when the enemy receives damage from the player or other sources
float AEnemyCharacter::TakeDamage(
	float			    damageAmount,    // The amount of damage being applied to the enemy
	FDamageEvent const& DamageEvent,     // Information about the damage event, such as the type of damage & any other data
	AController*		EventInstigator, // The controller responsible for causing the damage
	AActor*				DamageCauser)    // The actor that directly caused the damage
{
	// Reduce the enemy's health by the damage amount
	CurrentHealth -= damageAmount;

	// Check if the enemy's health has dropped to 0 or
	// below, & if so, handle death
	if (CurrentHealth <= 0.0f)
	{
		// Chance to play death sound
		// This is to add some variety to the audio feedback on death,
		// so it doesn't get too repetitive with many enemies dying
		if (DeathSound && (FMath::RandRange(0, 100) < DeathSoundChance))
		{
			// Play the death sound at the enemy's location with the specified pitch & attenuation settings
			UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), 1.0f, Pitch, 0.0f, SoundAttenuation);
		}

		// Set bIsDead to true to prevent any further actions or damage
		// from being applied to the enemy while they are dead
		IsDead = true;

		// If killed by the player character, increase kill count
		if (DamageCauser && DamageCauser->IsA(AGamesDev_Assignment2Character::StaticClass()))
		{
			// Cast the damage causer to the player character class to
			// access player-specific properties & methods
			AGamesDev_Assignment2Character* Player = Cast<AGamesDev_Assignment2Character>(DamageCauser);

			// Check if the cast was successful & the player reference is valid
			if (Player)
			{
				// Check if the game mode is valid & is of the correct class
				if (AGamesDev_Assignment2GameMode* GM = Cast<AGamesDev_Assignment2GameMode>(UGameplayStatics::GetGameMode(GetWorld())))
				{
					// Update the player's kill count in the game mode, & if they have
					// met the current objective, increase their movement speed & play
					// dialogue based on the current objective number
					if (GM->UpdateKills())
					{
						Player->GetCharacterMovement()->JumpZVelocity += Player->JumpZVelocityIncrement;
						Player->GetCharacterMovement()->AirControl += Player->AirControlIncrement;
						Player->GetCharacterMovement()->MaxWalkSpeed += Player->MaxWalkSpeedIncrement;

						int currentObjectiveNumber = GM->GetCurrentObjectiveNumber();

						// These checks are all seperated because there are different dialogue lines for
						// each objective, so we need to check the objective number for each one to play
						// the correct line. If they were all in the same if statement, it would only check
						// the first one & play that line for all objectives, which would be incorrect.
						if (currentObjectiveNumber == 1)
						{
							UGameplayStatics::PlaySound2D(this, Player->Dialogue1, 1.0f, 1.0f, 0.0f);
						}
						else if (currentObjectiveNumber == 1)
						{
							UGameplayStatics::PlaySound2D(this, Player->Dialogue2, 1.0f, 1.0f, 0.0f);
						}
						else if (currentObjectiveNumber == 1)
						{
							UGameplayStatics::PlaySound2D(this, Player->Dialogue3, 1.0f, 1.0f, 0.0f);
						}
						else if (currentObjectiveNumber == 1)
						{
							UGameplayStatics::PlaySound2D(this, Player->Dialogue4, 1.0f, 1.0f, 0.0f);
						}

					}

				}

				// If the player has overhealth, add the health regen on kill to the overhealth pool,
				// otherwise add it to current health & check if it exceeds max health to add to overhealth
				if (Player->CurrentOverHealth > 0)
				{
					Player->CurrentHealth = Player->MaxHealth;
					Player->CurrentOverHealth += Player->HealthRegenOnKill;
				}
				else
				{
					// Overhealth not yet in use, add to health first
					Player->CurrentHealth += Player->HealthRegenOnKill;

					if (Player->CurrentHealth > Player->MaxHealth)
					{
						// If health exceeds max health, calculate excess health and add it to overhealth pool
						float ExcessHealth = Player->CurrentHealth - Player->MaxHealth;

						Player->CurrentHealth = Player->MaxHealth;
						Player->CurrentOverHealth += ExcessHealth;
					}

				}

			}

		}

		// Disable movement and collision
		GetCharacterMovement()->DisableMovement();
		SetActorEnableCollision(false);

		// Play death animation montage
		if (EnemyDeathAnimation)
		{
			AnimInstance->Montage_Play(EnemyDeathAnimation);
		}
		
		// Timer to teleport actor after death animation finishes
		GetWorld()->GetTimerManager().SetTimer(DeathTimer, this, &AEnemyCharacter::TeleportAway, DeathDuration, false);

	}

	// Check health thresholds to play different damage sounds based on current health percentage,
	if (IsHealthLow())
	{
		if (LowHealthDamageSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, LowHealthDamageSound, GetActorLocation(), 0.3f, Pitch, 0.0f, SoundAttenuation);
		}
	}
	else if (CurrentHealth <= MaxHealth * 0.5f)
	{
		if (MediumHealthDamageSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, MediumHealthDamageSound, GetActorLocation(), 0.3f, Pitch, 0.0f, SoundAttenuation);
		}
	}
	else
	{
		if (HighHealthDamageSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, HighHealthDamageSound, GetActorLocation(), 0.3f, Pitch, 0.0f, SoundAttenuation);
		}
	}

	// Return the damage amount that was applied
	return DamageAmount;
}

// Method to respawn the enemy back up after being teleported away,
// which is called by a timer after the respawn duration
void AEnemyCharacter::Respawn()
{
	// Reset health
	CurrentHealth = MaxHealth;

	// Teleport enemy back up to where it was last, this is fine because there is
	// no nav mesh below the level, so it wouldn't be able to move or interact
	// with anything while teleported down, so we can just teleport it back up 
	// without needing to worry about it being in the wrong place
	FVector NewLocation = GetActorLocation();
	NewLocation.Z += 10000.0f;
	SetActorLocation(NewLocation);
	SetActorEnableCollision(true);

	// Update the IsDead flag to false to allow the enemy to start acting again after respawning
	IsDead = false;

	// Chance to play respawn sound
	if (SpawnSound && (FMath::RandRange(0, 100) < RespawnSoundChance))
	{
		// Play the respawn sound at the enemy's location with the specified pitch & attenuation settings
		UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, GetActorLocation(), 1.0f, Pitch, 0.0f, SoundAttenuation);
	}

	// Reenable movement and collision
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

}

// Method to update the enemy's movement speed based on the current walk speed variable,
void AEnemyCharacter::UpdateMovementSpeed()
{
	// Check if the character movement component exists before trying to set the speed
	if (GetCharacterMovement())
	{
		// Set the max walk speed to the current walk speed
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

// Method to update the enemy's behavior based on current health and ability states,
void AEnemyCharacter::UpdateBehavior()
{
	// Check if the enemy can use both shoot & attack abilities,
	// & switch behavior tree based on health thresholds
	if (bCanUseShoot && bCanUseAttack)
	{
		// If health is above the switch tree threshold, use the shooting
		// behavior tree, otherwise use the melee behavior tree
		if (CurrentHealth > MaxHealth * SwitchTreeThreshold)
		{
			// Check if the controller is valid before trying to switch behavior trees
			if (Controller)
			{
				// Cast the controller to the enemy AI controller class to access the SwitchBehaviorTree method,
				// then call that method to switch to the shooting behavior tree
				auto EnemyController = Cast<AEnemyAIController>(Controller);
				EnemyController->SwitchBehaviorTree(ShootBehaviorTree);
			}

		}
		else
		{
			// Check if the controller is valid before trying to switch behavior trees
			if (Controller)
			{
				// Cast the controller to the enemy AI controller class to access the SwitchBehaviorTree method,
				// then call that method to switch to the melee behavior tree
				auto EnemyController = Cast<AEnemyAIController>(Controller);
				EnemyController->SwitchBehaviorTree(BehaviorTree);
			}

		}

	}
	// If the enemy can only use one of the abilities, switch to the
	// corresponding behavior tree without checking health thresholds
	else if (bCanUseShoot && !bCanUseAttack)
	{
		// Check if the controller is valid before trying to switch behavior trees
		if (Controller)
		{
			// Cast the controller to the enemy AI controller class to access the SwitchBehaviorTree method,
			// then call that method to switch to the shooting behavior tree
			auto EnemyController = Cast<AEnemyAIController>(Controller);
			EnemyController->SwitchBehaviorTree(ShootBehaviorTree);
		}
	}
	// If the enemy can only use one of the abilities, switch to the
	// corresponding behavior tree without checking health thresholds
	else if (!bCanUseShoot && bCanUseAttack)
	{
		// Check if the controller is valid before trying to switch behavior trees
		if (Controller)
		{
			// Cast the controller to the enemy AI controller class to access the SwitchBehaviorTree method,
			// then call that method to switch to the melee behavior tree
			auto EnemyController = Cast<AEnemyAIController>(Controller);
			EnemyController->SwitchBehaviorTree(BehaviorTree);
		}
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	// Call the base class Tick to ensure any additional functionality is performed
	Super::Tick(DeltaTime);

	// Chance at idle sound per tick
	//if (!PlayerDetected && IdleSound && (FMath::RandRange(0, 100) < IdleSoundChance))
	//{
	//	// Play the idle sound at the enemy's location with the specified pitch & attenuation settings
	//	UGameplayStatics::PlaySoundAtLocation(this, IdleSound, GetActorLocation(), 1.0f, Pitch, 0.0f, SoundAttenuation);
	//}

}
