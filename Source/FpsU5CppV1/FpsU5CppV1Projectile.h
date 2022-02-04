// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FpsU5CppV1Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config=Game)
class AFpsU5CppV1Projectile : public AActor
{
	GENERATED_BODY()

public:
	AFpsU5CppV1Projectile();

	
	void SetProjectileDamage(float dmg);

	//void SetProjectileMaxSpeed(float maxSpeed);
	//void SetProjectileMaxSpeed(float maxSpeed);

	/** Sphere collision component */
	UPROPERTY(EditAnywhere, Category = Projectile)
		USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = Damage)
		TSubclassOf<UDamageType> BulletDamage;
	
	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; };
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = true))
		float InitialSpeed = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = true))
		float MaxSpeed = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = true))
		float ProjectileDamage = 20.0f;

	bool bCanApplyDamage;
};

