// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponAmmoType.h"
#include "BaseWeapon.generated.h"

USTRUCT(BlueprintType)
struct FPlayerWeapon
{
	GENERATED_BODY()

		/* Ammo */
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UWeaponAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int ProjectilesFiredPerShot = 1;
public:
	UWeaponAmmoType GetAmmoType() { return AmmoType; };
	int GetProjectilesFiredPerShot() {
		return ProjectilesFiredPerShot;
	};

};

UCLASS()
class FPSU5CPPV1_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



};
