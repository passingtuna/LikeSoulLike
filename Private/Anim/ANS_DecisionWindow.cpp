// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_DecisionWindow.h"
#include "CharacterDefaultBase.h"

void UANS_DecisionWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	DecisionChar = Cast<ACharacterDefaultBase>(MeshComp->GetOwner());

	if (IsValid(DecisionChar))
	{
		switch (StateType)
		{
		case EDecisionType::DT_Attack:
		{
			DecisionChar->SetIsAttacking(true);
		}
		break;
		case EDecisionType::DT_Avoid:
		{
			DecisionChar->SetIsAvoiding(true);
		}
		break;
		case EDecisionType::DT_Guard:
		{
			DecisionChar->SetIsGuarding(true);
		}
		break;
		case EDecisionType::DT_Bounce:
		{
			DecisionChar->SetIsBouncing(true);
		}
		break;
		case EDecisionType::DT_Parry:
		{
			DecisionChar->SetIsParrying(true);
		}
		break;
		}
	}
}

void UANS_DecisionWindow::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
}

void UANS_DecisionWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (IsValid(DecisionChar))
	{
		switch (StateType)
		{
		case EDecisionType::DT_Attack:
		{
			DecisionChar->SetIsAttacking(false);
		}
		break;
		case EDecisionType::DT_Avoid:
		{
			DecisionChar->SetIsAvoiding(false);
		}
		break;
		case EDecisionType::DT_Guard:
		{
			DecisionChar->SetIsGuarding(false);
		}
		break;
		case EDecisionType::DT_Bounce:
		{
			DecisionChar->SetIsBouncing(false);
		}
		break;
		case EDecisionType::DT_Parry:
		{
			DecisionChar->SetIsParrying(false);
		}
		break;
		}
	}
	DecisionChar = nullptr;
}
