// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_WeaponEvent.h"
#include "CharacterDefaultBase.h"
#include "WeaponDefaultBase.h"

void UAN_WeaponEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();
	if (!IsValid(Owner)) return;
	ACharacterDefaultBase* DecisionChar = Cast<ACharacterDefaultBase>(Owner);
	if (!DecisionChar) return;
	AWeaponDefaultBase * tempWeapon = DecisionChar->GetCurrentWeapon();
	if (tempWeapon)
	{
		tempWeapon->IncreaseWeaponEventNum(InputType);
	}
}