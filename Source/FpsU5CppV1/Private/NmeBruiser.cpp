// Fill out your copyright notice in the Description page of Project Settings.


#include "NmeBruiser.h"

#include "GameFramework/CharacterMovementComponent.h"

ANmeBruiser::ANmeBruiser()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	//GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}
