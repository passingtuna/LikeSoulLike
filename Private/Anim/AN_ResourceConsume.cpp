// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_ResourceConsume.h"
#include "CharacterDefaultBase.h"


void UAN_ResourceConsume::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();
	if (!IsValid(Owner)) return;

	ACharacterDefaultBase* DecisionChar = Cast<ACharacterDefaultBase>(Owner);
	if (!DecisionChar) return;
	DecisionChar->ActionConsumeProcess();
}
