// Fill out your copyright notice in the Description page of Project Settings.


#include "Medkit.h"
#include "FpsU5CppV1/FpsU5CppV1Character.h"

// Sets default values
AMedkit::AMedkit()
{
	OnActorBeginOverlap.AddDynamic(this, &AMedkit::OnOverlap);
}

void AMedkit::OnOverlap(AActor* MyOverlappedActor, AActor* OtherActor)
{
	if(OtherActor == nullptr || OtherActor == this)
		return;

	auto const MyCharacter = Cast<AFpsU5CppV1Character>(OtherActor);

	if (!MyCharacter || MyCharacter->GetHealth() >= 1.0f)
		return;

	MyCharacter->UpdateHealth(100.0f);
	Destroy();
}
