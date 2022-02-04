// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "FpsU5CppV1/FpsU5CppV1Character.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "AttackType.h"

// Sets default values
ANPC::ANPC() : RightFistCollisionBox(CreateDefaultSubobject<UBoxComponent>(TEXT("Fist Collision Box"))), bCanDamagePlayer(true)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	//GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	FullHealth = 100.0f;

	if (RightFistCollisionBox)
	{
		FVector const extent(8.0f);
		RightFistCollisionBox->SetBoxExtent(extent, false);
		RightFistCollisionBox->SetCollisionProfileName(TEXT("NoCollision"));
		
	}
}

void ANPC::SetMovementSpeed(float speed)
{
	GetCharacterMovement()->MaxWalkSpeed = speed;
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();

	auto const materialInstance = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);

	if(materialInstance)
	{
		materialInstance->SetVectorParameterValue("BodyColor", FLinearColor(1.0f, 0.0f, 0.0f, 0.0f));
		GetMesh()->SetMaterial(0, materialInstance);
	}

	if(RightFistCollisionBox)
	{
		FAttachmentTransformRules const rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
		RightFistCollisionBox->AttachToComponent(GetMesh(), rules, "had_r_socket");
		RightFistCollisionBox->SetRelativeLocation(FVector(-7.0f, 0.0f, 0.0f));

		//RightFistCollisionBox->OnComponentHit.AddDynamic(this, &ANPC::OnHit);
		RightFistCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ANPC::OnAttackOverlapBegin);
		RightFistCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ANPC::OnAttackOverlapEnd);
	}
	
}

void ANPC::UpdateHealth(float healthChange)
{
	health += healthChange;
	health = FMath::Clamp(health, 0.0f, FullHealth);
}

float ANPC::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                       AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Actor Taking Damage"));
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::FromInt(IsAwake ? 1 : 0));
	SetCanBeDamaged(false);
	UpdateHealth(-DamageAmount);

	if (FMath::IsNearlyZero(health, 0.0001f))
	{
		/*GetController()->SetCinematicMode(true, false, false, true, true);
		GetMesh()->SetSimulatePhysics(true);
		GetMovementComponent()->MovementState.bCanJump = false;
		*/
		Destroy();
	}

	return DamageAmount;
}

void ANPC::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr || !bCanDamagePlayer)
		return;

	bCanDamagePlayer = false;

	UGameplayStatics::ApplyPointDamage(OtherActor, GetMeleeDamage(), GetActorLocation(), Hit, nullptr, this, Damage);
}

void ANPC::OnAttackOverlapBegin(UPrimitiveComponent* const overlappedComponent, AActor* const otherActor,
	UPrimitiveComponent* otherComponent, int const otherBodyIndex, bool const fromSweep, FHitResult const& sweepResult)
{
	if (otherActor == nullptr || otherActor == this || otherComponent == nullptr || !bCanDamagePlayer)
		return;

	bCanDamagePlayer = false;

	UGameplayStatics::ApplyPointDamage(otherActor, GetMeleeDamage(), GetActorLocation(), sweepResult, nullptr, this, Damage);
}

void ANPC::OnAttackOverlapEnd(UPrimitiveComponent* const overlappedComponent, AActor* const otherActor,
	UPrimitiveComponent* otherComponent, int const otherBodyIndex)
{
	bCanDamagePlayer = true;
}

void ANPC::AttackStart()
{
	RightFistCollisionBox->SetCollisionProfileName("Fist");
	RightFistCollisionBox->SetNotifyRigidBodyCollision(true);
}

void ANPC::AttackEnd()
{
	RightFistCollisionBox->SetCollisionProfileName("NoCollision");
	RightFistCollisionBox->SetNotifyRigidBodyCollision(false);
	AttackType = UAttackType::NOTATTACKING;
}

void ANPC::SpawnProjectile()
{
	
	// try and fire a projectile
	if (ProjectileClass == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("No Projectile Class"));
		return;
	}
	UWorld* const World = GetWorld();
	if (!World) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("No World"));
		return;
	}

	
	const FRotator SpawnRotation = GetControlRotation();

	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	const FVector SpawnLocation = 
		(
			(RightFistCollisionBox != nullptr) ? 
			RightFistCollisionBox->GetComponentLocation() //+ SpawnRotation.RotateVector(attackOffset)
			:	GetActorLocation()) + SpawnRotation.RotateVector(AttackOffset);

	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	// spawn the projectile at the muzzle
	AFpsU5CppV1Projectile* projectile = World->SpawnActor<AFpsU5CppV1Projectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
	projectile->SetProjectileDamage(GetMissileDamage());
}

UBehaviorTree* ANPC::GetBehaviorTree() const
{
	return BehaviourTree;
}

float ANPC::GetMeleeDamage()
{
	return FMath::RandRange(MinMeleeDamage, MaxMeleeDamage);
}

float ANPC::GetMissileDamage()
{
	return FMath::RandRange(MinMissileDamage, MaxMissileDamage);
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

APatrolPath* ANPC::GetPatrolPath()
{
	return PatrolPath;
}

void ANPC::MeleeAttack()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("PAOW!"));

	AttackType = UAttackType::MELEE;

	//TODO: Add CD -> Check Task
	if(montage)
	{
		PlayAnimMontage(montage);
	}
}

void ANPC::MissileAttack() 
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("PEW!"));
	AttackType = UAttackType::MISSILE;
	//TODO: Add CD -> Check Task
	if (montage) 
	{
		PlayAnimMontage(montage);
	}
}

UAnimMontage* ANPC::GetMontage() const
{
	return montage;
}

float ANPC::GetHealth() const
{
	return health;
}

float ANPC::GetFullHealth() const
{
	return FullHealth;
}

void ANPC::SetHealth(float const value)
{
	health = value;
}

