// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_Inputable.h"
#include "CharacterDefaultBase.h"

void UAN_Inputable::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();
	if (!IsValid(Owner)) return;

	ACharacterDefaultBase* DecisionChar = Cast<ACharacterDefaultBase>(Owner);
	if (!DecisionChar) return;
	if (IsEnd)
	{
		DecisionChar->ActionEnd();
	}
	else
	{
		DecisionChar->SetIsMoveable(IsMoveable);
		DecisionChar->SetIsAvoidable(IsAvoidable);
		DecisionChar->SetIsAttackable(IsAttackable);
		DecisionChar->SetIsRunable(IsRunable);
		DecisionChar->SetIsGrabable(IsGrabable);
	}
}