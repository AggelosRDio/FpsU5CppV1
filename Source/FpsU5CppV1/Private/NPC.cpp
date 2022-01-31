// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "FpsU5CppV1/FpsU5CppV1Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ANPC::ANPC() : RightFistCollisionBox(CreateDefaultSubobject<UBoxComponent>(TEXT("Fist Collision Box"))), bCanDamagePlayer(true)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	//GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	FullHealth = 100.0f;

	if (RightFistCollisionBox)
	{
		FVector const extent(8.0f);
		RightFistCollisionBox->SetBoxExtent(extent, false);
		RightFistCollisionBox->SetCollisionProfileName(TEXT("NoCollision"));
		
	}
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

	UGameplayStatics::ApplyPointDamage(OtherActor, MeleeDamage, GetActorLocation(), Hit, nullptr, this, Damage);
}

void ANPC::OnAttackOverlapBegin(UPrimitiveComponent* const overlappedComponent, AActor* const otherActor,
	UPrimitiveComponent* otherComponent, int const otherBodyIndex, bool const fromSweep, FHitResult const& sweepResult)
{
	if (otherActor == nullptr || otherActor == this || otherComponent == nullptr || !bCanDamagePlayer)
		return;

	bCanDamagePlayer = false;

	UGameplayStatics::ApplyPointDamage(otherActor, MeleeDamage, GetActorLocation(), sweepResult, nullptr, this, Damage);
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
}

UBehaviorTree* ANPC::GetBehaviorTree() const
{
	return BehaviourTree;
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
	//TODO: Add CD -> Check Task
	if(montage)
	{
		PlayAnimMontage(montage);
	}
}

void ANPC::MissileAttack() 
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("PEW!"));
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

