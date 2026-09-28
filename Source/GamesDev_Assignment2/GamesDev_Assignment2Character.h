#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
#include "ECreatureType.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "GamesDev_Assignment2Character.generated.h"

class  USpringArmComponent;
class  UCameraComponent;
class  UInputAction;
class  UInputMappingContext;
struct FInputActionValue;

UCLASS()
class AGamesDev_Assignment2Character : public ACharacter
{
	GENERATED_BODY()

public:
	AGamesDev_Assignment2Character();

	// ***********************
	// Public Properties
	// ***********************

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* Dialogue1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* Dialogue2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* Dialogue3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* Dialogue4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* Dialogue5;

	// Health
	UPROPERTY(BlueprintReadWrite, Category = "Health")
	float CurrentHealth;

	UPROPERTY(BlueprintReadWrite, Category = "Health")
	float CurrentOverHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxOverHealth = 1000.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Health")
	float HealthRegenOnKill = 50.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Health")
	int Kills;

	// Objectives
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Objective1 = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Objective2 = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Objective3 = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Objective4 = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxKills = Objective1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Objective1Updated = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Objective2Updated = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Objective3Updated = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Objective4Updated = false;

	// Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpZVelocityIncrement = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AirControlIncrement = 0.9f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWalkSpeedIncrement = 80.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RespawnTimer = 7.0f;

	// Animation States
	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	bool bWantsToFire = false;

	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	bool bWasHit = false;

	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	bool bIsDead = false;

	// ***********************
	// Public Functions
	// ***********************

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	bool IsCharacterDead();
	void HandleDeath();

	ECreatureType GetCreatureType() const { return TypeOfCreature; }

	UFUNCTION()
	void SwitchCameraToRandomEnemy();

	UFUNCTION()
	USoundBase* PickRandomMusic();

	UFUNCTION(BlueprintCallable)
	bool HasPlayerReachedGoal() const;

	void Tick(float DeltaTime);

private:
	// ***********************
	// AI Perception
	// ***********************

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAIPerceptionStimuliSourceComponent* StimuliSource;

	void SetupStimuliSource();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// ***********************
	// Core Stats
	// ***********************

	UPROPERTY(EditAnywhere, Category = "Stats")
	float LaunchDirectionStrengthFactor = 0.9f;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float ImpulseStrength = 10000.0f;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float LocationXGoal = 14200.0f;

	UPROPERTY(EditAnywhere, Category = "Stats")
	int WalkSpeed = 600;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float SpawnSoundTime = 5.0f;

	// ***********************
	// Audio
	// ***********************

	UPROPERTY(EditAnywhere, Category = "Audio")
	float MusicVolume = 0.7f;

	UPROPERTY(EditAnywhere, Category = "Audio")
	float RareHitsoundChance = 0.0025f;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundBase* SpawnSound1;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundBase* SpawnSound2;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundBase* FireSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundBase* RareHitSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundBase* JumpSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundBase* DeathDialogueSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundBase* Music1;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundBase* Music2;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundBase* Music3;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundBase* Music4;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundBase* Music5;

	FTimerHandle SpawnSoundTimer;

	// ***********************
	// Camera
	// ***********************

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* FollowCamera;

	FTimerHandle CameraSwitchTimer;
	AActor* CurrentCameraTarget;

	// ***********************
	// Input
	// ***********************

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MouseLookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* EscapeAction;

	// ***********************
	// Animation
	// ***********************

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* DeathMontage;

	// ***********************
	// Combat / Effects
	// ***********************

	UPROPERTY(EditAnywhere)
	float DamageAmount = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Weapon Effects")
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, Category = "Weapon Effects")
	UParticleSystem* BulletTrail;

	UPROPERTY(EditAnywhere, Category = "Weapon Effects")
	UParticleSystem* HitCharacterEffect;

	UPROPERTY(EditAnywhere, Category = "Weapon Effects")
	UParticleSystem* HitWorldEffect;

	// ***********************
	// UI
	// ***********************

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> HealthWidgetClass;

	UPROPERTY()
	UUserWidget* HealthWidget;

	// ***********************
	// Creature Type
	// ***********************

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	ECreatureType TypeOfCreature = ECreatureType::CT_CHARACTER;

	// ***********************
	// Movement & Actions
	// ***********************

	void Move  (const FInputActionValue& Value);
	void Look  (const FInputActionValue& Value);
	void Escape(const FInputActionValue& Value);
	void Fire  ();

	void DoMove(float Right, float Forward);
	void DoLook(float Yaw, float Pitch);
	void DoJumpStart();
	void DoJumpEnd();

protected:
	// ***********************
	// Engine Overrides
	// ***********************

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};