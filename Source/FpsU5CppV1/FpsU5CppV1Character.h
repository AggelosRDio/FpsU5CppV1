#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "Components/TimelineComponent.h"
#include <FpsU5CppV1/Public/ActorStructs.h>
#include <FpsU5CppV1/Public/BaseWeapon.h>
#include "FpsU5CppV1Character.generated.h"



class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;

USTRUCT(BlueprintType)
struct FCharacterAmmo
{
	GENERATED_BODY()

		/* Ammo */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FActorIntStats Shells;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FActorIntStats Bullets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FActorIntStats Energy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FActorIntStats Rockets;

	FCharacterAmmo()
	{
		Shells.MaxValue = 24;
		Shells.Value = Shells.MaxValue;
		Bullets.MaxValue = 180;
		Bullets.Value = Bullets.MaxValue;
		Energy.MaxValue = 250;
		Energy.Value = Energy.MaxValue;
		Rockets.MaxValue = 13;
		Rockets.Value = Rockets.MaxValue;
	}

public:
	int GetCurrentAmmoValue(UWeaponAmmoType ammoType);
	int GetMaxAmmoValue(UWeaponAmmoType ammoType);
	float GetAmmoValuePercentage(UWeaponAmmoType ammoType);
	bool UpdateAmmoValue(int value, UWeaponAmmoType ammoType);
};

USTRUCT(BlueprintType)
struct FCharacterVitals 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health, meta = (AllowPrivateAccess = true))
		float FullHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health, meta = (AllowPrivateAccess = true))
		float CurrentHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health, meta = (AllowPrivateAccess = true))
		float HealthPercentage = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = true))
		float PreviousHealth;
public:
	float GetCurrentHealth() const { return CurrentHealth; };
	float GetMaxHealth() const { return FullHealth; };
	float GetHealthPercentage() const { return HealthPercentage; };
	void SetCurrentHealth(float const value) { CurrentHealth = value; };
	void UpdateCurrentHealth(float const value) { 
		CurrentHealth += value;
		CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, FullHealth);
		HealthPercentage = CurrentHealth / FullHealth;
	};

	//FCharacterVitals() { }
};

UCLASS(config=Game)
class AFpsU5CppV1Character : public ACharacter
{
	GENERATED_BODY()

public:
	AFpsU5CppV1Character();
	
	FCharacterVitals GetCharacterVitals() const { return CharacterVitals; };

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AFpsU5CppV1Projectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		uint8 bUsingMotionControllers : 1;

	/* Health & HUD */

	UFUNCTION(BlueprintPure, Category = Health)
		float GetHealth();
	UFUNCTION(BlueprintPure, Category = Health)
		FText GetHealthIntText();

	UFUNCTION(BlueprintPure, Category = Weapon)
		float GetEquippedWeaponAmmo();
	UFUNCTION(BlueprintPure, Category = Weapon)
		FText GetEquippedWeaponAmmoText();

	UFUNCTION()
		void DamageTimer();

	UFUNCTION()
		void SetDamageState();

	UFUNCTION(BlueprintPure, Category = Health)
		bool PlayFlash();

	UPROPERTY(EditAnywhere, Category = Health, meta = (AllowPrivateAccess = true))
		UMaterialInterface* GunDefaultMaterial;

	UPROPERTY(EditAnywhere, Category = Health, meta = (AllowPrivateAccess = true))
		UMaterialInterface* GunOverheatMaterial;

	UFUNCTION(BlueprintCallable, Category = Health)
		void UpdateHealth(float healthChange);

	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UPROPERTY(EditAnywhere, Category = Health, meta = (AllowPrivateAccess = true))
		TArray<ABaseWeapon*> Weapons;

	UPROPERTY(EditAnywhere, Category = Health, meta = (AllowPrivateAccess = true))
		int WeaponIndex;

	void SwitchWeapon();

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
		void SwitchWeaponMesh(int index);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/** Fires a projectile. */
	void OnFire();

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	void OnSlide();

	void OnSlideRelease();

	bool bIsSliding = false;

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

	/* Mobility */
	virtual void Landed(const FHitResult& Hit) override;
	virtual void Jump() override;

private:
	bool bIsLanded;
	int nJumpCount;
	FVector DoubleJumpVector;

	bool CanFire();


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

	/* Perception */
	class UAIPerceptionStimuliSourceComponent* stimulus;

	void SetupStimulus();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = true))
		UAnimMontage* montage;

	void onMeleeAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterStats, meta = (AllowPrivateAccess = true))
		FCharacterVitals CharacterVitals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStats", meta = (AllowPrivateAccess = true))
		float PreviousMagic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = true))
		bool redFlash;

	UPROPERTY(EditAnywhere, Category = "CharacterStats", meta = (AllowPrivateAccess = true))
		FTimeline MyTimeline;

	UPROPERTY(EditAnywhere, Category = "CharacterStats", meta = (AllowPrivateAccess = true))
		FTimerHandle MemberTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterStats, meta = (AllowPrivateAccess = true))
		FCharacterAmmo AmmoInventory;

	float CurveFloatValue;
	float TimelineValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = true))
		FPlayerWeapon EquippedWeapon;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = true))
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

};

