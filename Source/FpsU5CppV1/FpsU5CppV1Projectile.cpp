// Copyright Epic Games, Inc. All Rights Reserved.

#include "FpsU5CppV1Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AFpsU5CppV1Projectile::AFpsU5CppV1Projectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFpsU5CppV1Projectile::OnHit);		// set up a notification for when this component hits something blocking


	CollisionComp->SetGenerateOverlapEvents(true);

	/*CollisionComp->SetCollisionProfileName(TEXT("Trigger"));*/
	//CollisionComp->SetCollisionProfileName(TEXT(""))

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = InitialSpeed;//3000.f;
	ProjectileMovement->MaxSpeed = MaxSpeed;//3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AFpsU5CppV1Projectile::OnOverlapBegin);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &AFpsU5CppV1Projectile::OnOverlapEnd);

	bCanApplyDamage = true;
}

void AFpsU5CppV1Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherComp->IsSimulatingPhysics())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, TEXT("Projectile Hit"));
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

			//auto npc = Cast<>

			Destroy();
		}
	}


}

void AFpsU5CppV1Projectile::SetProjectileDamage(float dmg) {
	ProjectileDamage = dmg;
}

void AFpsU5CppV1Projectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, TEXT("Projectile Overlap Begin"));
	//if (!bCanApplyDamage) return;

	if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr)
		return;

	bCanApplyDamage = false;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, TEXT("Projectile Overlap Begin"));

	UGameplayStatics::ApplyPointDamage(OtherActor, ProjectileDamage, GetActorLocation(), SweepResult, nullptr, this, BulletDamage);

}

void AFpsU5CppV1Projectile::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, TEXT("Projectile Overlap End"));
	Destroy();
}