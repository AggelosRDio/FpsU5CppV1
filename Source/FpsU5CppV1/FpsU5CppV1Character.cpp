// Copyright Epic Games, Inc. All Rights Reserved.

#include "FpsU5CppV1Character.h"
//#include "AiTags.h"
#include "AiTags.h"
#include "FpsU5CppV1Projectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "UserHudWidget.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Components/WidgetComponent.h"
#include "Perception/AISense_Hearing.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AFpsU5CppV1Character

AFpsU5CppV1Character::AFpsU5CppV1Character() //: WidgetComponent(CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthValue")))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;

	SetupStimulus();

	//if(WidgetComponent)
	//{
	//	WidgetComponent->SetupAttachment(GetRootComponent());
	//	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	//	WidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 85.0f));

	//	static ConstructorHelpers::FClassFinder<UUserWidget> widgetClass(TEXT("/Game/UI/UserHud_BP"));

	//	if (widgetClass.Succeeded())
	//	{
	//		WidgetComponent->SetWidgetClass(widgetClass.Class);
	//	}

	//	auto const uw = Cast<UUserHudWidget>();

	//	if (uw)
	//	{
	//		uw->AddToViewport();
	//	}
	//	

	////	//widgetClass->
	//}
}

void AFpsU5CppV1Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	MyTimeline.TickTimeline(DeltaSeconds);
}

void AFpsU5CppV1Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}
	
	bIsWeaponFiring = false;
	nJumpCount = 0;
	bIsLanded = true;
	DoubleJumpVector = FVector(0.0f, 0.0f, 300.0f);

	FullHealth = 1000.0f;
	CurrentHealth = FullHealth;
	HealthPercentage = 1.0f;
	PreviousHealth = HealthPercentage;
	SetCanBeDamaged(true);
	//bCanBeDamaged = true;

	FullMagic = 100.0f;
	CurrentMagic = 100.0f;
	MagicPercentage = 1.0f;
	PreviousMagic = MagicPercentage;
	
	bCanUseMagic = true;

	if(MagicCurve)
	{
		FOnTimelineFloat timelineCallback;
		FOnTimelineEventStatic timelineFinishedCallback;

		timelineCallback.BindUFunction(this, FName("SetMagicState"));
		timelineFinishedCallback.BindUFunction(this, FName("SetMagicState"));

		MyTimeline.AddInterpFloat(MagicCurve, timelineCallback);
		MyTimeline.SetTimelineFinishedFunc(timelineFinishedCallback);
		
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFpsU5CppV1Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFpsU5CppV1Character::OnFire);

	PlayerInputComponent->BindAction("AlternateFire", IE_Pressed, this, &AFpsU5CppV1Character::OnAlternateFire);
	PlayerInputComponent->BindAction("AlternateFire", IE_Released, this, &AFpsU5CppV1Character::OnAlternateFireRelease);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AFpsU5CppV1Character::OnDash);
	PlayerInputComponent->BindAction("Dash", IE_Released, this, &AFpsU5CppV1Character::OnDashRelease);

	PlayerInputComponent->BindAction("AttackMelee", IE_Pressed, this, &AFpsU5CppV1Character::onMeleeAttack);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AFpsU5CppV1Character::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AFpsU5CppV1Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFpsU5CppV1Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFpsU5CppV1Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFpsU5CppV1Character::LookUpAtRate);
}

void AFpsU5CppV1Character::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AFpsU5CppV1Character::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AFpsU5CppV1Character::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void AFpsU5CppV1Character::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void AFpsU5CppV1Character::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFpsU5CppV1Character::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFpsU5CppV1Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFpsU5CppV1Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AFpsU5CppV1Character::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AFpsU5CppV1Character::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AFpsU5CppV1Character::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AFpsU5CppV1Character::TouchUpdate);
		return true;
	}
	
	return false;
}

/* Dash */
void AFpsU5CppV1Character::OnDash()
{
	if (DashCounter <= 0) {

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Dash Counter empty"));
		return;
	}

	if (bIsDashing)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Already Dashing"));
		return;
	}
	
	GetForwardDashVector();
	GetRightDashVector();

	LaunchCharacter(DashVector, false, false);

	//SetActorLocation(DashVector, true);

	DashCounter--;
	bIsDashOnCooldown = true;

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Dashed"));
	ResetDashVector();
}

void AFpsU5CppV1Character::OnDashRelease()
{
	if (!bIsDashOnCooldown) return;

	GetWorld()->GetTimerManager().SetTimer(DashCooldownManager, this, &AFpsU5CppV1Character::RefreshDash, fDashDelay, false);
}

void AFpsU5CppV1Character::ResetDashVector()
{
	DashVector = FVector(0, 0, 300.0f);
}

void AFpsU5CppV1Character::RefreshDash()
{
	if (DashCounter < MaxDashCounter)
	{
		DashCounter++;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Dash Counter Increased"));

		bIsDashOnCooldown = DashCounter < MaxDashCounter;
		if (bIsDashOnCooldown) OnDashRelease();

		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(DashCooldownManager);
	bIsDashOnCooldown = false;
}

FVector AFpsU5CppV1Character::GetForwardDashVector()
{
	const auto world = GetWorld();

	if (world == nullptr) return DashVector;

	auto data = world->GetFirstPlayerController()->PlayerInput->GetKeysForAxis("MoveForward");

	for (const FInputAxisKeyMapping item : data)
	{
		const auto bIsKeyForwardPressed = UGameplayStatics::GetPlayerController(world, 0)->IsInputKeyDown(item.Key);

		if (!bIsKeyForwardPressed) continue;

		DashVector = (GetActorForwardVector() * fDashMultiplicationFactor * item.Scale) + DashVector;
	}

	return DashVector;
}

FVector AFpsU5CppV1Character::GetRightDashVector()
{
	const auto world = GetWorld();

	if (world == nullptr) return DashVector;

	auto data = world->GetFirstPlayerController()->PlayerInput->GetKeysForAxis("MoveRight");

	for (const FInputAxisKeyMapping item : data)
	{
		const auto bIsKeyPressed = UGameplayStatics::GetPlayerController(world, 0)->IsInputKeyDown(item.Key);

		if (!bIsKeyPressed) continue;

		DashVector = (GetActorRightVector() * fDashMultiplicationFactor * item.Scale) + DashVector;
	}

	return DashVector;
}



/* Fire */
void AFpsU5CppV1Character::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass == nullptr || FMath::IsNearlyZero(CurrentMagic, 0.001f) || !bCanUseMagic)
		return;


	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		if (bUsingMotionControllers)
		{
			const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
			const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
			World->SpawnActor<AFpsU5CppV1Projectile>(ProjectileClass, SpawnLocation, SpawnRotation);
		}
		else
		{
			const FRotator SpawnRotation = GetControlRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<AFpsU5CppV1Projectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			//AFpsU5CppV1Projectile* projectile = World->SpawnActor<AFpsU5CppV1Projectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			//projectile->GetCollisionComp()->SetGenerateOverlapEvents(true);

		}
	}

	// try and play the sound if specified
	/*if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1.0f, this, 0.0f, Tags::NoiseTag);
	}*/

	// try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}

	//TODO: Reference Timers below to fix Dash
	MyTimeline.Stop();
	GetWorldTimerManager().ClearTimer(MagicTimerHandle);
	SetMagicChange(-20.0f);
	GetWorldTimerManager().SetTimer(MagicTimerHandle, this, &AFpsU5CppV1Character::UpdateMagic, 5.0f, false);

}

void AFpsU5CppV1Character::OnAlternateFire()
{
	bIsWeaponFiring = true;

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Emerald, TEXT("Alternate Fire"));
}

void AFpsU5CppV1Character::OnAlternateFireRelease()
{
	bIsWeaponFiring = false;

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Emerald, TEXT("Alternate Fire Released"));
}

void AFpsU5CppV1Character::SetupStimulus()
{
	stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	stimulus->RegisterWithPerceptionSystem();

}

void AFpsU5CppV1Character::onMeleeAttack()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Melee Attack Placeholder"));

	/*if (montage)
	{
		PlayAnimMontage(montage);
	}*/
}

float AFpsU5CppV1Character::GetCurrentHealth() const
{
	return CurrentHealth;
}

float AFpsU5CppV1Character::GetMaxHealth() const
{
	return FullHealth;
}

void AFpsU5CppV1Character::SetCurrentHealth(float const value)
{
	CurrentHealth = value;
}

float AFpsU5CppV1Character::GetHealth()
{
	return HealthPercentage;
}

FText AFpsU5CppV1Character::GetHealthIntText()
{
	const int hp = FMath::RoundHalfFromZero(HealthPercentage * 100);
	const FString hps = FString::FromInt(hp);
	const FString healthHud = hps + FString(TEXT("%"));
	FText hpText = FText::FromString(healthHud);

	return hpText;
}

float AFpsU5CppV1Character::GetMagic()
{
	return MagicPercentage;
}

FText AFpsU5CppV1Character::GetMagicIntText()
{
	const int mp = FMath::RoundHalfFromZero(MagicPercentage * 100);
	const FString mps = FString::FromInt(mp);
	const FString fullMps = FString::FromInt(FullMagic);
	const FString magicHud = mps + FString(TEXT("/") + fullMps);
	FText mpText = FText::FromString(magicHud);

	return mpText;
}

void AFpsU5CppV1Character::DamageTimer()
{
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AFpsU5CppV1Character::SetDamageState, 2.0f, false);
}

void AFpsU5CppV1Character::SetDamageState()
{
	SetCanBeDamaged(true);
}

void AFpsU5CppV1Character::SetMagicValue()
{
	TimelineValue = MyTimeline.GetPlaybackPosition();
	CurveFloatValue = PreviousMagic + MagicValue * MagicCurve->GetFloatValue(TimelineValue);
	CurrentMagic = CurveFloatValue * FullHealth;
	CurrentMagic = FMath::Clamp(CurrentMagic, 0.0f, FullMagic);
	MagicPercentage = CurveFloatValue;
	MagicPercentage = FMath::Clamp(MagicPercentage, 0.0f, 1.0f);
}

void AFpsU5CppV1Character::SetMagicState()
{
	bCanUseMagic = true;
	MagicValue = 0.0f;

	if(GunDefaultMaterial)
	{
		FP_Gun->SetMaterial(0, GunDefaultMaterial);
	}
}

void AFpsU5CppV1Character::SetMagicChange(float value)
{
	bCanUseMagic = false;
	PreviousMagic = MagicPercentage;
	MagicValue = value / FullMagic;

	if(GunOverheatMaterial)
	{
		FP_Gun->SetMaterial(0, GunOverheatMaterial);
	}

	MyTimeline.PlayFromStart();
}

void AFpsU5CppV1Character::UpdateMagic()
{
	PreviousMagic = MagicPercentage;
	MagicPercentage = CurrentMagic / FullMagic;
	MagicValue = 1.0f;
	MyTimeline.PlayFromStart();
}

bool AFpsU5CppV1Character::PlayFlash()
{
	if(redFlash)
	{
		redFlash = false;
		return true;
	}

	return false;
}

//void AFpsU5CppV1Character::ReceivePointDamage(float Damage, const UDamageType* DamageType, FVector HitLocation,
//	FVector HitNormal, UPrimitiveComponent* HitComponent, FName BoneName, FVector ShotFromDirection,
//	AController* InstigatedBy, AActor* DamageCauser, const FHitResult& HitInfo)
//{
//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Taking Damage"));
//	SetCanBeDamaged(false);
//	redFlash = true;
//	UpdateHealth(-Damage);
//	// start invincibility timer
//	DamageTimer();
//}

float AFpsU5CppV1Character::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	SetCanBeDamaged(false);
	//bCanBeDamaged = false;
	redFlash = true;
	UpdateHealth(-DamageAmount);
	DamageTimer();
	return DamageAmount;
}

void AFpsU5CppV1Character::UpdateHealth(float healthChange)
{
	CurrentHealth += healthChange;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, FullHealth);
	HealthPercentage = CurrentHealth / FullHealth;
}

void AFpsU5CppV1Character::Landed(const FHitResult& Hit)
{
	bIsLanded = true;
	nJumpCount = 0;
	Super::Landed(Hit);
}

void AFpsU5CppV1Character::Jump()
{
	if (bIsLanded)
	{
		Super::Jump();
		nJumpCount++;
		bIsLanded = false;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Emerald, TEXT("Jump"));
		return;
	}

	if (nJumpCount > 1) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Can't Jump"));
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.0f,FColor::Cyan, TEXT("Double Jump"));

	LaunchCharacter(DoubleJumpVector, false, false);

	nJumpCount++;

}
