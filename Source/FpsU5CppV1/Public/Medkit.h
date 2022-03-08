// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Medkit.generated.h"

UCLASS()
class FPSU5CPPV1_API AMedkit : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MakeEditWidget = true, AllowPrivateAccess = true))
		float MedkitValue = 10.0f;

public:	
	// Sets default values for this actor's properties
	AMedkit();

	UFUNCTION()
		void OnOverlap(AActor* MyOverlappedActor, AActor* OtherActor);
};
