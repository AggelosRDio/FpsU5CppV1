// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "NPC.generated.h"

class APatrolPath;
UCLASS()
class FPSU5CPPV1_API ANPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	APatrolPath* GetPatrolPath();

	void MeleeAttack();
	void MissileAttack();

	UAnimMontage* GetMontage() const;

	float GetHealth() const;
	float GetFullHealth() const;
	void SetHealth(float const value);

	UFUNCTION(BlueprintCallable, Category = Health)
		void UpdateHealth(float healthChange);

	virtual void AttackStart();
	virtual void AttackEnd();

	virtual UBehaviorTree* GetBehaviorTree() const;
	
	float GetMeleeDamage() { return MeleeDamage; };

	float GetMeleeRange() { return MeleeRange; };

	float GetMissileDamage() { return MissileDamage; };

	float GetMissileRange() { return MissileRange; };

	bool GetCanAttackMelee() { return CanAttackMelee; };
	bool GetCanAttackRanged() { return CanAttackRanged; };

	virtual bool GetIsAwake() { return IsAwake; };

	virtual void SetIsAwake(bool value) { IsAwake = value; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = true))
		bool IsAwake = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = true))
		APatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = true))
		UAnimMontage* montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage, meta = (AllowPrivateAccess = true))
		bool CanAttackMelee = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage, meta = (AllowPrivateAccess = true))
		float MeleeDamage = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage, meta = (AllowPrivateAccess = true))
		float MeleeRange = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage, meta = (AllowPrivateAccess = true))
		float MissileDamage = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage, meta = (AllowPrivateAccess = true))
		float MissileRange = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage, meta = (AllowPrivateAccess = true))
		bool CanAttackRanged = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage, meta = (AllowPrivateAccess = true))
		bool Ambush = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = true))
		UBehaviorTree* BehaviourTree;
	
	/* Health & Damage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health, meta = (AllowPrivateAccess = true))
		float health = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health, meta = (AllowPrivateAccess = true))
		float FullHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = true))
		class UBoxComponent* RightFistCollisionBox;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnAttackOverlapBegin(UPrimitiveComponent* const overlappedComponent, AActor* const otherActor, UPrimitiveComponent* otherComponent, int const otherBodyIndex, bool const fromSweep, FHitResult const& sweepResult);
	UFUNCTION()
		void OnAttackOverlapEnd(UPrimitiveComponent* const overlappedComponent, AActor* const otherActor, UPrimitiveComponent* otherComponent, int const otherBodyIndex);

	UPROPERTY(EditAnywhere, Category = Damage)
		TSubclassOf<UDamageType> Damage;

	bool bCanDamagePlayer;
};
