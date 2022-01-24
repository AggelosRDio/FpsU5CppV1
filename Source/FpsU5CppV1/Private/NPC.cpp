// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	//GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	FullHealth = 100.0f;
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
}

void ANPC::UpdateHealth(float healthChange)
{
	health += healthChange;
	health = FMath::Clamp(health, 0.0f, FullHealth);
}

float ANPC::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                       AActor* DamageCauser)
{
	//return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Actor Taking Damage"));
	SetCanBeDamaged(false);
	UpdateHealth(-DamageAmount);
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Actor Health: " + FText::FromString(FString::FromInt(health))));

	if (FMath::IsNearlyZero(health, 0.0001f))
	{
		Destroy();
	}

	return DamageAmount;
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
	if(montage)
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

