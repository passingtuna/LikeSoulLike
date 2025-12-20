// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_AvoidMovement.h"
#include "CharacterDefaultBase.h"


void UANS_AvoidMovement::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	DecisionChar = Cast<ACharacterDefaultBase>(MeshComp->GetOwner());
	if (DecisionChar)
	{
		MoveDirection = DecisionChar->GetLastInputDir();
	}
}

void UANS_AvoidMovement::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (DecisionChar)
	{
		DecisionChar->AvoidMovement(MoveDirection, MovementSpeed * FrameDeltaTime);
	}
}

void UANS_AvoidMovement::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	DecisionChar = nullptr;
}
