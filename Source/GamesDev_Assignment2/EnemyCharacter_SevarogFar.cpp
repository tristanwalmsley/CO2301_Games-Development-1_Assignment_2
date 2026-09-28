#include "EnemyCharacter_SevarogFar.h"
#include "Camera/CameraComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"

AEnemyCharacter_SevarogFar::AEnemyCharacter_SevarogFar()
{
    //*******************
    // MESH
    //*******************
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObj(
        TEXT("/Game/Enemies/Meshes/Sevarog")
    );

    if (MeshObj.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(MeshObj.Object);
        GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -160.0f));
        GetMesh()->SetRelativeRotation(FRotator(0.0f, -130.0f, 0.0f));
    }

    //*******************
    // ANIM INSTANCE
    //*******************
    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(
        TEXT("/Game/Enemies/Animations/Sevarog_AnimBlueprint")
    );

    if (AnimBP.Succeeded())
    {
        GetMesh()->SetAnimInstanceClass(AnimBP.Class);
    }

    //*******************
    // ANIMATION MONTAGES
    //*******************

    // Enemy Attack
    static ConstructorHelpers::FObjectFinder<UAnimMontage> EnemyAttackAnimationMontageFile(
        TEXT("/Game/Enemies/Animations/Swing1_Medium_Montage")
    );

    if (EnemyAttackAnimationMontageFile.Succeeded())
    {
        EnemyAttackAnimation = EnemyAttackAnimationMontageFile.Object;
    }

    // Enemy Death
    static ConstructorHelpers::FObjectFinder<UAnimMontage> EnemyDeathAnimationMontageFile(
        TEXT("/Game/Enemies/Animations/Death_front_Montage")
    );

    if (EnemyDeathAnimationMontageFile.Succeeded())
    {
        EnemyDeathAnimation = EnemyDeathAnimationMontageFile.Object;
    }

    // Player Death Reaction
    static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerDeathReactionAnimationMontageFile(
        TEXT("/Game/Enemies/Animations/Emote_Boogie_Montage")
    );

    if (PlayerDeathReactionAnimationMontageFile.Succeeded())
    {
        PlayerDeathReactionAnimationMontage = PlayerDeathReactionAnimationMontageFile.Object;
    }

    //*******************
    // SOUNDS
    //*******************

    // Death
    static ConstructorHelpers::FObjectFinder<USoundBase> DeathSoundFile(
        TEXT("/Game/Enemies/Audio/SoundCues/Sevarog_Death")
    );

    if (DeathSoundFile.Succeeded())
    {
        DeathSound = DeathSoundFile.Object;
    }

    // Low Health Damage
    static ConstructorHelpers::FObjectFinder<USoundBase> LowHealthDamageFile(
        TEXT("/Game/Enemies/Audio/SoundCues/Sevarog_Effort_Pain")
    );

    if (LowHealthDamageFile.Succeeded())
    {
        LowHealthDamageSound = LowHealthDamageFile.Object;
    }

    // Medium Health Damage Sound
    static ConstructorHelpers::FObjectFinder<USoundBase> MediumHealthDamageFile(
        TEXT("/Game/Enemies/Audio/SoundWaves/Sevarog_Effort_Pain_03")
    );

    if (MediumHealthDamageFile.Succeeded())
    {
        MediumHealthDamageSound = MediumHealthDamageFile.Object;
    }

    // High Health Damage Sound
    static ConstructorHelpers::FObjectFinder<USoundBase> HighHealthDamageFile(
        TEXT("/Game/Enemies/Audio/SoundCues/Sevarog_Effort_PainHeavy")
    );

    if (HighHealthDamageFile.Succeeded())
    {
        HighHealthDamageSound = HighHealthDamageFile.Object;
    }

    // Attack Sound
    static ConstructorHelpers::FObjectFinder<USoundBase> AttackSoundFile(
        TEXT("/Game/Enemies/Audio/SoundCues/Sevarog_Effort_Swing")
    );

    if (AttackSoundFile.Succeeded())
    {
        AttackSound = AttackSoundFile.Object;
    }

    // Spawn Sound
    static ConstructorHelpers::FObjectFinder<USoundBase> SpawnSoundFile(
        TEXT("/Game/Enemies/Audio/SoundCues/Sevarog_Revive")
    );

    if (SpawnSoundFile.Succeeded())
    {
        SpawnSound = SpawnSoundFile.Object;
    }

    // Idle Sound
    static ConstructorHelpers::FObjectFinder<USoundBase> IdleSoundFile(
        TEXT("/Game/Enemies/Audio/SoundCues/Sevarog_Status_Idle")
    );

    if (IdleSoundFile.Succeeded())
    {
        IdleSound = IdleSoundFile.Object;
    }

    // Sound Attenuation
    static ConstructorHelpers::FObjectFinder<USoundAttenuation> SoundAttenuationFile(
        TEXT("/Game/Enemies/Audio/SoundAttenuation")
    );

    if (SoundAttenuationFile.Succeeded())
    {
        SoundAttenuation = SoundAttenuationFile.Object;
    }

}

void AEnemyCharacter_SevarogFar::BeginPlay()
{
	Super::BeginPlay();

    // Stats
	HealthFleeThreshold          = 0.4f;
	AttackCooldown	             = 10.0f;
	DamageAmount	             = 300.0f;
	MaxHealth		             = 300.0f;
	HealthRegenRate	             = 25.0f;
	HealthRegenInterval          = 1.0f;
	RespawnDuration	             = 30.0f;
	FireRate		             = 0.15f;
	Pitch				         = 3.0f;
    ProjectileDamage             = 50.0f;
    ProjectileInitialSpeed       = 2000.0f;
    ProjectileMaxSpeed           = 2000.0f;
    ProjectileLifeSpan           = 5.0f;
    RangedAttackRange            = 2000.0f;
    MeleeAttackRange             = 300.0f;
    bCanUseAttack                = false;
    bCanUseShoot                 = true;
    SwitchTreeThreshold          = 0.5f;

    // Camera
    EnemyCamera_Location         = FVector(330.0f, 0.0f, 100.0f);
    EnemyCamera_Rotation         = FRotator(180.0f, -10.0f, 180.0f);

    // Point Light
    PointLight_Location          = FVector(20.0f, 0.0f, 80.0f);
    PointLight_Rotation          = FRotator(0.0f, 0.0f, 0.0f);
    PointLight_LightColor        = FLinearColor::Yellow;
    PointLight_Intensity         = 15.0f;
    PointLight_AttenuationRadius = 75.0f;
    PointLight_CastShadows       = false;
    PointLight_SourceRadius      = 100.0f;

    // Point Light 2
    PointLight2_Location          = FVector(0.0f, 0.0f, 160.0f);
    PointLight2_Rotation          = FRotator(0.0f, 0.0f, 0.0f);
    PointLight2_LightColor        = FLinearColor(255, 244, 169);
    PointLight2_Intensity         = 25.0f;
    PointLight2_AttenuationRadius = 200.0f;
    PointLight2_CastShadows       = false;
    PointLight2_SourceRadius      = 100.0f;
    PointLight2_SoftSourceRadius  = 530.0f;
    PointLight2_SourceLength      = 110.0f;

    // Spot Light
    SpotLight_Location           = FVector(50.0f, 0.0f, 60.0f);
    SpotLight_Rotation           = FRotator(-45.0f, 0.0f, 0.0f);
    SpotLight_LightColor         = FLinearColor::Red;
    SpotLight_Intensity          = 500.0f;
    SpotLight_AttenuationRadius  = 1500.0f;
    SpotLight_CastShadows        = false;
    SpotLight_SourceRadius       = 20.0f;
    SpotLight_InnerConeAngle     = 10.0f;
    SpotLight_OuterConeAngle     = 40.0f;

    // Movemement
	WalkSpeed                    = 300.0f;
	SprintSpeed                  = 800.0f;
	UpdateMovementSpeed();

    AnimInstance = GetMesh()->GetAnimInstance();

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
