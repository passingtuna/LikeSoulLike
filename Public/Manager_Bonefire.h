// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Manager_Bonefire.generated.h"

/**
 * 
 */
class ABoneFire;
class ADropItem;
class ACharacterPlayableBase;
UCLASS()
class LIKESOULLIKE_API UManager_Bonefire : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:
	TMap<FName, TWeakObjectPtr<ABoneFire>> mapBonefire;
	TSet<FName> ActivatedBonfireIDs;
	FName LastVisitBonefireName;
	ADropItem * DropSoul;


	const FName GetLastVisitBonefireName() { return LastVisitBonefireName; };
	void SetLastVisitBonefireName(FName lastVisitBonefireName) { LastVisitBonefireName = lastVisitBonefireName;};
	void AddBonefire(TWeakObjectPtr<ABoneFire> bonfire);
	void AddLitedBonefire(FName name);
	void UpdateLastRestBonefire(FName name) { LastVisitBonefireName = name; };
	void WarpTargetBonefire(const FName name , ACharacterPlayableBase* pc);

	TSet<FName> GetActivateBonfireList() { return ActivatedBonfireIDs; };

	void RespawnLastBonefire(ACharacterPlayableBase* pc);

	void SetDropSoul(ADropItem* dropSoul);
};
