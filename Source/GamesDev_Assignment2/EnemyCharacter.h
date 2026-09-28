#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyProjectile.h"
#include "ECreatureType.h"
#include "EnemyCharacter.generated.h"

class UCameraComponent;

UCLASS()
class GAMESDEV_ASSIGNMENT2_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// ***********************
	// Constructor & Overrides
	// ***********************

	AEnemyCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// ***********************
	// STATS
	// ***********************

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsFleeing = false;

	UPROPERTY(EditAnywhere, Category = "AI")
	bool PlayerDetected = false;

	UPROPERTY(EditAnywhere, Category = "AI")
	bool CanAttackPlayer = false;

	UPROPERTY(BlueprintReadWrite, Category = "AI")
	bool CanDealDamage = false;

	UPROPERTY(EditAnywhere, Category = "AI")
	class AGamesDev_Assignment2Character* PlayerREF;

	UPROPERTY(EditAnywhere, Category = "AI")
	class USphereComponent* PlayerCollisionDetection;

	UPROPERTY(EditAnywhere, Category = "AI")
	class USphereComponent* PlayerAttackCollisionDetection;

	UPROPERTY(EditAnywhere, Category = "AI")
	class AController* EnemyAIController;

	// ***********************
	// SOUNDS
	// ***********************

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundBase* LowHealthDamageSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundBase* MediumHealthDamageSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundBase* HighHealthDamageSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundBase* AttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundBase* SpawnSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundBase* IdleSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	class USoundAttenuation* SoundAttenuation;

	float DeathSoundChance = 20.0f;
	float RespawnSoundChance = 25.0f;
	float IdleSoundChance = 0.001f;

	// ***********************
	// ANIMATION
	// ***********************

	UPROPERTY(EditAnywhere, Category = "Animation")
	class UAnimMontage* EnemyAttackAnimation;

	UPROPERTY(EditAnywhere, Category = "Animation")
	class UAnimMontage* EnemyDeathAnimation;

	UPROPERTY(EditAnywhere, Category = "Animation")
	class UAnimMontage* PlayerDeathReactionAnimationMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	class UAnimInstance* AnimInstance;

	// ***********************
	// PUBLIC STATS & COMBAT FLAGS
	// ***********************

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bCanAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bCanShoot = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bCanUseAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bCanUseShoot = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float SwitchTreeThreshold = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AttackCooldown = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float DamageAmount = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth = 500.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Stats")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float WalkSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float SprintSpeed = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Pitch = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float RangedAttackRange = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MeleeAttackRange = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool IsDead = false;

	// ***********************
	// TIMERS
	// ***********************

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FTimerHandle AttackCooldownTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FTimerHandle BoogieTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float BoogieTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FTimerHandle HealthRegenTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float HealthRegenRate = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float HealthRegenInterval = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Stats")
	FTimerHandle RespawnTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float RespawnDuration = 30.0f;

	UPROPERTY(EditAnywhere, Category = "Stats")
	FTimerHandle DeathTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float DeathDuration = 0.7f;

	// ***********************
	// AI / BEHAVIOR TREES
	// ***********************

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* ShootBehaviorTree;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UBehaviorTree* GetShootBehaviorTree() const { return ShootBehaviorTree; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetFleeThreshold() const { return FleeThreshold; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetAttackThreshold() const { return AttackThreshold; }

	// ***********************
	// METHODS
	// ***********************

	UFUNCTION()
	void UpdateBehavior();

	UFUNCTION(BlueprintCallable)
	virtual void ReactToPlayerDeath();

	UFUNCTION(BlueprintCallable)
	virtual void ShootProjectile();

	UFUNCTION(BlueprintCallable)
	virtual void SetSprinting(bool bSprint);

	UFUNCTION(BlueprintCallable)
	virtual void RegenerateHealth();

	UFUNCTION(BlueprintCallable)
	virtual void MeleeAttackPlayer(AGamesDev_Assignment2Character* PlayerCharacter);

	UFUNCTION()
	virtual void ResetAttack();

	UFUNCTION()
	virtual void ResetShoot();

	UFUNCTION(BlueprintCallable)
	virtual void DealDamage(AGamesDev_Assignment2Character* PlayerCharacter);

	UFUNCTION(BlueprintCallable)
	virtual void TeleportAway();

	UFUNCTION(BlueprintCallable)
	virtual void Respawn();

	UFUNCTION()
	bool IsEnemyDead() const { return IsDead; }

	UFUNCTION(BlueprintCallable)
	virtual void UpdateMovementSpeed();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsHealthLow() const { return CurrentHealth <= MaxHealth * HealthFleeThreshold; }

	virtual ECreatureType GetCreatureType() const { return TypeOfCreature; }

protected:
	// ***********************
	// ENGINE OVERRIDES
	// ***********************

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// ***********************
	// COMBAT
	// ***********************

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<AEnemyProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float FireRate = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float ProjectileDamage = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float ProjectileInitialSpeed = 2000.0f;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float ProjectileMaxSpeed = 2000.0f;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float ProjectileLifeSpan = 50.0f;

	FTimerHandle FireTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	ECreatureType TypeOfCreature = ECreatureType::CT_SEVAROG;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	int FleeThreshold = -4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	int AttackThreshold = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float HealthFleeThreshold = 0.2f;

	// ***********************
	// CAMERA
	// ***********************

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* EnemyCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector EnemyCamera_Location = FVector(0.f, 0.f, 50.f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FRotator EnemyCamera_Rotation = FRotator(-10.f, 0.f, 0.f);

	// ***********************
	// LIGHTING
	// ***********************

	// POINT LIGHT

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UPointLightComponent* PointLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector PointLight_Location = FVector(20.0f, 0.0f, 160.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FRotator PointLight_Rotation = FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FLinearColor PointLight_LightColor = FLinearColor::Green;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float PointLight_Intensity = 5000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float PointLight_AttenuationRadius = 84.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool PointLight_CastShadows = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float PointLight_SourceRadius = 0.0f;

	// POINT LIGHT 2

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UPointLightComponent* PointLight2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector PointLight2_Location = FVector(0.0f, 0.0f, 160.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FRotator PointLight2_Rotation = FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FLinearColor PointLight2_LightColor = FLinearColor(255, 244, 169);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float PointLight2_Intensity = 4.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float PointLight2_AttenuationRadius = 200.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool PointLight2_CastShadows = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float PointLight2_SourceRadius = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float PointLight2_SoftSourceRadius = 530.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float PointLight2_SourceLength = 110.0f;

	// SPOT LIGHT

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USpotLightComponent* SpotLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector SpotLight_Location = FVector(23.0f, -10.0f, 164.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FRotator SpotLight_Rotation = FRotator(0.0f, -10.0f, 0.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FLinearColor SpotLight_LightColor = FLinearColor::Red;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float SpotLight_Intensity = 100000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float SpotLight_AttenuationRadius = 700.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool SpotLight_CastShadows = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float SpotLight_SourceRadius = 55.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float SpotLight_InnerConeAngle = 20.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float SpotLight_OuterConeAngle = 45.0f;

private:
	// ***********************
	// AI INTERNALS
	// ***********************

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* StimuliSource;

	void SetupStimuliSource();
};





//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/Character.h"
//#include "BehaviorTree/BehaviorTree.h"
//#include "EnemyProjectile.h"
//#include "ECreatureType.h"
//#include "EnemyCharacter.generated.h"
//
//class UCameraComponent;
//
//UCLASS()
//class GAMESDEV_ASSIGNMENT2_API AEnemyCharacter : public ACharacter
//{
//	GENERATED_BODY()
//
//public:
//	AEnemyCharacter();
//
//	virtual void Tick(float DeltaTime) override;
//	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
//
//	// =========================
//	// ATTRIBUTES
//	// =========================
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
//	bool bIsFleeing = false;
//
//	UPROPERTY(EditAnywhere, Category = "AI")
//	bool PlayerDetected = false;
//
//	UPROPERTY(EditAnywhere, Category = "AI")
//	bool CanAttackPlayer = false;
//
//	UPROPERTY(BlueprintReadWrite, Category = "AI")
//	bool CanDealDamage = false;
//
//	UPROPERTY(EditAnywhere, Category = "AI")
//	class AGamesDev_Assignment2Character* PlayerREF;
//
//	UPROPERTY(EditAnywhere, Category = "AI")
//	class USphereComponent* PlayerCollisionDetection;
//
//	UPROPERTY(EditAnywhere, Category = "AI")
//	class USphereComponent* PlayerAttackCollisionDetection;
//
//	UPROPERTY(EditAnywhere, Category = "AI")
//	class AController* EnemyAIController;
//
//	// =========================
//	// ANIMATION
//	// =========================
//
//	UPROPERTY(EditAnywhere, Category = "Animation")
//	class UAnimMontage* EnemyAttackAnimation;
//
//	UPROPERTY(EditAnywhere, Category = "Animation")
//	class UAnimMontage* EnemyDeathAnimation;
//
//	UPROPERTY(EditAnywhere, Category = "Animation")
//	class UAnimMontage* PlayerDeathReactionAnimationMontage;
//
//	UPROPERTY(EditAnywhere, Category = "Animation")
//	class UAnimInstance* AnimInstance;
//
//	// =========================
//	// STATS
//	// =========================
//
//protected:
//	// =========================
//	// COMBAT
//	// =========================
//
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
//	TSubclassOf<AEnemyProjectile> ProjectileClass;
//
//	FTimerHandle FireTimer;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
//	ECreatureType TypeOfCreature = ECreatureType::CT_SEVAROG;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
//	int FleeThreshold = -4;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
//	int AttackThreshold = 4;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
//	float HealthFleeThreshold = 0.2f;
//
//	UPROPERTY(EditDefaultsOnly, Category = "Combat")
//	float FireRate = 2.0f;
//	
//	UPROPERTY(EditAnywhere, Category = "Projectile")
//	float ProjectileDamage = 50.0f;
//
//	UPROPERTY(EditAnywhere, Category = "Projectile")
//	float ProjectileInitialSpeed = 2000.0f;
//
//	UPROPERTY(EditAnywhere, Category = "Projectile")
//	float ProjectileMaxSpeed = 2000.0f;
//
//	UPROPERTY(EditAnywhere, Category = "Projectile")
//	float ProjectileLifeSpan = 50.0f;
//
//	// =========================
//	// CAMERA
//	// =========================
//
//	UPROPERTY(VisibleAnywhere)
//	UCameraComponent* EnemyCamera;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	FVector EnemyCamera_Location = FVector(0.f, 0.f, 50.f);
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	FRotator EnemyCamera_Rotation = FRotator(-10.f, 0.f, 0.f);
//
//	// =========================
//	// POINT LIGHT
//	// =========================
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	class UPointLightComponent* PointLight;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	FVector PointLight_Location = FVector(20.0f, 0.0f, 160.0f);
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	FRotator PointLight_Rotation = FRotator(0.0f, 0.0f, 0.0f);
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	FLinearColor PointLight_LightColor = FLinearColor::Green;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	float PointLight_Intensity = 5000.0f;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	float PointLight_AttenuationRadius = 84.0f;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	bool PointLight_CastShadows = false;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	float PointLight_SourceRadius = 0.0f;
//
//	// =========================
//	// POINT LIGHT 2
//	// =========================
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	class UPointLightComponent* PointLight2;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	FVector PointLight2_Location = FVector(0.0f, 0.0f, 160.0f);
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	FRotator PointLight2_Rotation = FRotator(0.0f, 0.0f, 0.0f);
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	FLinearColor PointLight2_LightColor = FLinearColor(255, 244, 169);
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	float PointLight2_Intensity = 4.0f;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	float PointLight2_AttenuationRadius = 200.0f;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	bool PointLight2_CastShadows = false;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	float PointLight2_SourceRadius = 100.0f;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	float PointLight2_SoftSourceRadius = 530.0f;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	float PointLight2_SourceLength = 110.0f;
//
//	// =========================
//	// SPOT LIGHT
//	// =========================
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	class USpotLightComponent* SpotLight;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	FVector SpotLight_Location = FVector(23.0f, -10.0f, 164.0f);
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	FRotator SpotLight_Rotation = FRotator(0.0f, -10.0f, 0.0f);
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	FLinearColor SpotLight_LightColor = FLinearColor::Red;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	float SpotLight_Intensity = 100000.0f;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	float SpotLight_AttenuationRadius = 700.0f;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	bool SpotLight_CastShadows = true;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	float SpotLight_SourceRadius = 55.0f;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	float SpotLight_InnerConeAngle = 20.0f;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	float SpotLight_OuterConeAngle = 45.0f;
//
//public:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
//	bool bCanAttack = true;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
//	bool bCanShoot = true;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
//	float SwitchTreeThreshold = 0.5f;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
//	bool bCanUseAttack = true;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
//	bool bCanUseShoot = true;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
//	float AttackCooldown = 1.0f;
//
//	UFUNCTION()
//	void UpdateBehavior();
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
//	FTimerHandle AttackCooldownTimer;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
//	FTimerHandle BoogieTimer;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
//	float BoogieTime = 5.0f;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
//	float DamageAmount = 200.0f;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
//	float MaxHealth = 500.0f;
//
//	UPROPERTY(BlueprintReadWrite, Category = "Stats")
//	float CurrentHealth;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
//	float WalkSpeed = 400.0f;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
//	float SprintSpeed = 1500.0f;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
//	float Pitch = 1.0f;
//
//	// Health Regen
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
//	FTimerHandle HealthRegenTimer;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
//	float HealthRegenRate = 50.0f;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
//	float HealthRegenInterval = 5.0f;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
//	float RangedAttackRange = 1200.0f;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
//	float MeleeAttackRange = 300.0f;
//
//	// Death / Respawn
//	UPROPERTY(EditAnywhere, Category = "Stats")
//	FTimerHandle RespawnTimer;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
//	float RespawnDuration = 30.0f;
//
//	UPROPERTY(EditAnywhere, Category = "Stats")
//	FTimerHandle DeathTimer;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
//	float DeathDuration = 0.7f;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
//	bool IsDead = false;
//
//	// =========================
//	// SOUNDS
//	// =========================
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
//	class USoundBase* DeathSound;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
//	class USoundBase* LowHealthDamageSound;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
//	class USoundBase* MediumHealthDamageSound;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
//	class USoundBase* HighHealthDamageSound;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
//	class USoundBase* AttackSound;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
//	class USoundBase* SpawnSound;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
//	class USoundBase* IdleSound;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
//	class USoundAttenuation* SoundAttenuation;
//
//	// =========================
//	// AI
//	// =========================
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
//	UBehaviorTree* BehaviorTree;
//
//	UFUNCTION(BlueprintCallable, BlueprintPure)
//	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
//	// =========================
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
//	UBehaviorTree* ShootBehaviorTree;
//
//	UFUNCTION(BlueprintCallable, BlueprintPure)
//	UBehaviorTree* GetShootBehaviorTree() const { return ShootBehaviorTree; }
//
//	UFUNCTION(BlueprintCallable, BlueprintPure)
//	int GetFleeThreshold() const { return FleeThreshold; }
//
//	UFUNCTION(BlueprintCallable, BlueprintPure)
//	int GetAttackThreshold() const { return AttackThreshold; }
//
//	// =========================
//	// METHODS
//	// =========================
//
//	UFUNCTION(BlueprintCallable)
//	virtual void ReactToPlayerDeath();
//
//	UFUNCTION(BlueprintCallable)
//	virtual void ShootProjectile();
//
//	UFUNCTION(BlueprintCallable)
//	virtual void SetSprinting(bool bSprint);
//
//	UFUNCTION(BlueprintCallable)
//	virtual void RegenerateHealth();
//
//	UFUNCTION(BlueprintCallable)
//	bool IsHealthLow() const { return CurrentHealth <= MaxHealth * HealthFleeThreshold; }
//
//	virtual ECreatureType GetCreatureType() const { return TypeOfCreature; }
//
//	UFUNCTION(BlueprintCallable)
//	virtual void MeleeAttackPlayer(AGamesDev_Assignment2Character* PlayerCharacter);
//
//	UFUNCTION()
//	virtual void ResetAttack();
//
//	UFUNCTION()
//	virtual void ResetShoot();
//
//	UFUNCTION(BlueprintCallable)
//	virtual void DealDamage(AGamesDev_Assignment2Character* PlayerCharacter);
//
//	UFUNCTION(BlueprintCallable)
//	virtual void TeleportAway();
//
//	UFUNCTION(BlueprintCallable)
//	virtual void Respawn();
//
//	UFUNCTION()
//	bool IsEnemyDead() const { return IsDead; }
//
//	UFUNCTION()
//	virtual void UpdateMovementSpeed();
//
//protected:
//	virtual void BeginPlay() override;
//	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
//
//private:
//
//	// =========================
//	// AI INTERNALS
//	// =========================
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
//	class UAIPerceptionStimuliSourceComponent* StimuliSource;
//
//	void SetupStimuliSource();
//};