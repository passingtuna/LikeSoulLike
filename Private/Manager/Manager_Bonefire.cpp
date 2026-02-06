// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager_Bonefire.h"
#include "BoneFire.h"
#include "CharacterPlayableBase.h"
#include "DropItem.h"
#include "SoulLikeController.h"



void UManager_Bonefire::AddBonefire(TWeakObjectPtr<ABoneFire> bonfire)
{
	mapBonefire.Add(bonfire->Name,bonfire);
}

void UManager_Bonefire::AddLitedBonefire(FName name)
{
	ActivatedBonfireIDs.Add(name);
}

void UManager_Bonefire::WarpTargetBonefire(FName name, ACharacterPlayableBase* pc)
{
	if (!ActivatedBonfireIDs.Find(name)) return;
	TWeakObjectPtr<ABoneFire> * TargetBoneFire = mapBonefire.Find(name);
	if (!TargetBoneFire) return;
	if (!TargetBoneFire->IsValid()) return;
	ABoneFire* BoneFire = TargetBoneFire->Get();
	pc->SetActorTransform(BoneFire->GetRespawnTransform());
}
void UManager_Bonefire::RespawnLastBonefire(ACharacterPlayableBase* pc)
{
	if (!ActivatedBonfireIDs.Find(LastVisitBonefireName)) return;
	TWeakObjectPtr<ABoneFire>* TargetBoneFire = mapBonefire.Find(LastVisitBonefireName);
	if (!TargetBoneFire) return;
	if (!TargetBoneFire->IsValid()) return;
	ABoneFire* BoneFire = TargetBoneFire->Get();
	pc->SetActorTransform(BoneFire->GetRespawnTransform());
	pc->GetPlayerController()->SetControlRotation(BoneFire->GetRespawnTransform().Rotator());
}


void UManager_Bonefire::SetDropSoul(ADropItem* dropSoul)
{
	if (dropSoul == nullptr) return;
	if (DropSoul) DropSoul->Destroy();
	DropSoul = dropSoul;
}