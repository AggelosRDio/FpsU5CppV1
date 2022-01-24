// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "Components/TimelineComponent.h"
#include "FpsU5CppV1Character.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class AFpsU5CppV1Character : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* FP_MuzzleLocation;

	/** Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* VR_Gun;

	/** Location on VR gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* VR_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* L_MotionController;

public:
	AFpsU5CppV1Character();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AFpsU5CppV1Projectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint8 bUsingMotionControllers : 1;

protected:
	
	/** Fires a projectile. */
	void OnFire();

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/* Mobility */
protected:
	virtual void Landed(const FHitResult& Hit) override;
	virtual void Jump() override;

private:
	bool bIsLanded;
	int nJumpCount;
	FVector DoubleJumpVector;


	/* DASH */
	void OnDash();
	void OnDashRelease();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dash, meta = (AllowPrivateAccess = true))
		float fDashDelay = 2.0f;

	FTimerHandle DashCooldownManager;
	bool bIsDashOnCooldown = false;

	bool bIsDashing = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dash, meta = (AllowPrivateAccess = true))
		float fDashMultiplicationFactor = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dash, meta = (AllowPrivateAccess = true))
		int DashCounter = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dash, meta = (AllowPrivateAccess = true))
		int MaxDashCounter = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dash, meta = (AllowPrivateAccess = true))
		FVector DashVector = FVector(0.0f, 0.0f, 300.0f);;
	FVector GetForwardDashVector();
	FVector GetRightDashVector();
	void ResetDashVector();
	void RefreshDash();

		
	/* Weapon Firing */

	void OnAlternateFire();
	void OnAlternateFireRelease();

	bool bIsWeaponFiring;

	/* Ammo */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo, meta = (AllowPrivateAccess = true))
		int ShellAmmo = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo, meta = (AllowPrivateAccess = true))
		int ShellAmmoCap = 24;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo, meta = (AllowPrivateAccess = true))
		int BulletAmmo = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo, meta = (AllowPrivateAccess = true))
		int BulletAmmoCap = 180;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo, meta = (AllowPrivateAccess = true))
		int EnergyAmmo = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo, meta = (AllowPrivateAccess = true))
		int EnergyAmmoCap = 250;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo, meta = (AllowPrivateAccess = true))
		int RocketAmmo = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo, meta = (AllowPrivateAccess = true))
		int RocketAmmoCap = 13;

	/* Perception */
	class UAIPerceptionStimuliSourceComponent* stimulus;

	void SetupStimulus();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = true))
		UAnimMontage* montage;

	void onMeleeAttack();

	/* Health & HUD */
public:
	float GetCurrentHealth() const;
	float GetMaxHealth() const;
	void SetCurrentHealth(float const value);

private:
	//class UWidgetComponent* WidgetComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health, meta = (AllowPrivateAccess = true))
		float FullHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health, meta = (AllowPrivateAccess = true))
		float CurrentHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health, meta = (AllowPrivateAccess = true))
		float HealthPercentage = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = true))
		float PreviousHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health, meta = (AllowPrivateAccess = true))
		float FullMagic = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health, meta = (AllowPrivateAccess = true))
		float CurrentMagic = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic", meta = (AllowPrivateAccess = true))
		float MagicValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health, meta = (AllowPrivateAccess = true))
		float MagicPercentage = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = true))
		float PreviousMagic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = true))
		bool redFlash;

	UPROPERTY(EditAnywhere, Category = "Health", meta = (AllowPrivateAccess = true))
		UCurveFloat* MagicCurve;

	UPROPERTY(EditAnywhere, Category = "Health", meta = (AllowPrivateAccess = true))
		FTimeline MyTimeline;

	UPROPERTY(EditAnywhere, Category = "Health", meta = (AllowPrivateAccess = true))
		FTimerHandle MemberTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Health", meta = (AllowPrivateAccess = true))
		FTimerHandle MagicTimerHandle;

	float CurveFloatValue;
	float TimelineValue;
	bool bCanUseMagic;

public:
	UFUNCTION(BlueprintPure, Category = Health)
		float GetHealth();
	UFUNCTION(BlueprintPure, Category = Health)
		FText GetHealthIntText();
	UFUNCTION(BlueprintPure, Category = Health)
		float GetMagic();
	UFUNCTION(BlueprintPure, Category = Health)
		FText GetMagicIntText();

	UFUNCTION()
		void DamageTimer();

	UFUNCTION()
		void SetDamageState();

	UFUNCTION()
		void SetMagicValue();

	UFUNCTION()
		void SetMagicState();

	UFUNCTION()
		void SetMagicChange(float value);

	UFUNCTION()
		void UpdateMagic();

	UFUNCTION(BlueprintPure, Category = Health)
		bool PlayFlash();

	UPROPERTY(EditAnywhere, Category = Health, meta = (AllowPrivateAccess = true))
		UMaterialInterface* GunDefaultMaterial;

	UPROPERTY(EditAnywhere, Category = Health, meta = (AllowPrivateAccess = true))
		UMaterialInterface* GunOverheatMaterial;

	
	//UFUNCTION()
	//	void ReceivePointDamage(float Damage, const UDamageType* DamageType, FVector HitLocation, FVector HitNormal, UPrimitiveComponent* HitComponent, FName BoneName, FVector ShotFromDirection, AController* InstigatedBy, AActor* DamageCauser, const FHitResult& HitInfo);
		//void ReceivePointDamage(float Damage, const UDamageType* DamageType, FVector HitLocation, FVector HitNormal, UPrimitiveComponent* HitComponent, FName BoneName, FVector ShotFromDirection, AController* InstigatedBy, AActor* DamageCauser, const FHitResult& HitInfo);
	UFUNCTION(BlueprintCallable, Category = Health)
		void UpdateHealth(float healthChange);
protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

};

