// Fill out your copyright notice in the Description page of Project Settings.


#include "CampFire.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
ACampFire::ACampFire()
{
	MyBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));
	MyBoxComponent->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	RootComponent = MyBoxComponent;

	Fire = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("My Fire"));
	Fire->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Fire->SetupAttachment(RootComponent);

	MyBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACampFire::OnOverlapBegin);
	MyBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ACampFire::OnOverlapEnd);

	bCanApplyDamage = true;
}

void ACampFire::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr || OtherActor == this || OtherComp == nullptr) return;

	bCanApplyDamage = true;
	MyCharacter = Cast<AActor>(OtherActor);
	MyHit = SweepResult;
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ACampFire::ApplyFireDamage, 2.2f, true, 0.0f);
}

void ACampFire::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	bCanApplyDamage = false;
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
}

void ACampFire::ApplyFireDamage()
{
	if (!bCanApplyDamage) return;

	UGameplayStatics::ApplyPointDamage(MyCharacter, DamageValue, GetActorLocation(), MyHit, nullptr, this, FireDamage);
}
