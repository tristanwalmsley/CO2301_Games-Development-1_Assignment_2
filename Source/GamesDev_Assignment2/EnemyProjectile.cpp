#include "EnemyProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values - constructor
AEnemyProjectile::AEnemyProjectile()
{
	// Collision component
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere_Component"));
    CollisionComp->InitSphereRadius(8.0f);
    CollisionComp->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComp->SetCollisionObjectType(ECC_WorldDynamic);
    CollisionComp->SetCollisionResponseToAllChannels(ECR_Block);
    CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    RootComponent = CollisionComp;

	// Mesh component
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile_Mesh"));
    MeshComp->SetupAttachment(RootComponent);
    MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Movement component
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile_Movement"));
    ProjectileMovement->InitialSpeed = 1800.f;
    ProjectileMovement->MaxSpeed = 1800.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->ProjectileGravityScale = 0.f;

	// Add the OnHit function to the collision component's hit event
    CollisionComp->OnComponentHit.AddDynamic(this, &AEnemyProjectile::OnHit);
}

// Called when the game starts or when spawned
void AEnemyProjectile::BeginPlay()
{
	// Call the base class BeginPlay to ensure any additional setup is performed
    Super::BeginPlay();

	// Ensure ProjectileMovement is valid, if not, try to find it
    if (!ProjectileMovement)
    {
        ProjectileMovement = FindComponentByClass<UProjectileMovementComponent>();
    }

}

// Called every frame
void AEnemyProjectile::Tick(float DeltaTime)
{
	// Call the base class Tick function to ensure any additional functionality is preserved
    Super::Tick(DeltaTime);
}

// Function called when the projectile hits something, which
// applies damage to the hit actor & destroys the projectile
void AEnemyProjectile::OnHit(
	UPrimitiveComponent* HitComp,   // The component that was hit
	AActor* OtherActor,             // The actor that was hit
	UPrimitiveComponent* OtherComp, // The specific component of the hit actor that was hit
	FVector NormalImpulse,          // The impulse applied to the hit component as a result of the hit
	const FHitResult& Hit)          // Additional information about the hit, such as the location & normal of the impact
{
	// Check if the hit actor is valid and not the projectile itself
    if (OtherActor && OtherActor != this)
    {
        // Apply damage
        UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, nullptr);
    }

	// Destroy the projectile after hitting something to prevent it from applying damage multiple times
    Destroy();
}

// Function to initialise the projectile's properties, such as damage, speed, & lifespan
void AEnemyProjectile::InitProjectile(float InDamage, float InInitialSpeed, float InMaxSpeed, float InLifeSpan)
{
	// Set the projectile's damage to the provided value
    Damage = InDamage;

	// Ensure ProjectileMovement is valid, if not, try to find it
    if (ProjectileMovement)
    {
		// Set the projectile's initial speed, max speed, & velocity based on the provided values
        ProjectileMovement->InitialSpeed = InInitialSpeed;
        ProjectileMovement->MaxSpeed     = InMaxSpeed;
        ProjectileMovement->Velocity     = GetActorForwardVector() * InInitialSpeed;
    }

	// Set the projectile to destroy itself after the specified lifespan
    // to prevent it from existing indefinitely in the world
    SetLifeSpan(InLifeSpan);
}