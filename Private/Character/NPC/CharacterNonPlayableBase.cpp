// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterNonPlayableBase.h"

// Sets default values
ACharacterNonPlayableBase::ACharacterNonPlayableBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CharacterCamp = ECharacterCamp::CC_Monster;
}

// Called when the game starts or when spawned
void ACharacterNonPlayableBase::BeginPlay()
{
	MaxState.HP = 20000;
	Super::BeginPlay();


	CurrentState.HP = MaxState.HP;
	CurrentState.Mana = MaxState.Mana;
	CurrentState.Stemina = MaxState.Stemina;
	CurrentState.Toughness = MaxState.Toughness;
	MaxState.Toughness = 1;
	MaxState.Weight = 100;
}

// Called every frame
void ACharacterNonPlayableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterNonPlayableBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ACharacterNonPlayableBase::ModifyCurrentHealth(int param)
{

	UpdateLockOnUI();
}