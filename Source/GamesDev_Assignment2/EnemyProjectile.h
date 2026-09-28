#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyProjectile.generated.h"

UCLASS()
class GAMESDEV_ASSIGNMENT2_API AEnemyProjectile : public AActor
{
	GENERATED_BODY()

public:
	// ***********************
	// Constructor
	// ***********************

	// Sets default values for this actor's properties
	AEnemyProjectile();

	// ***********************
	// Engine Overrides
	// ***********************

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ***********************
	// Setup
	// ***********************

	void InitProjectile(float InDamage, float InInitialSpeed, float InMaxSpeed, float InLifeSpan);

protected:
	// ***********************
	// Engine Overrides
	// ***********************

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// ***********************
	// Collision
	// ***********************

	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

	// ***********************
	// Combat
	// ***********************

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float LifeSpanSeconds = 5.0f;

	// ***********************
	// Components
	// ***********************

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovement;
};