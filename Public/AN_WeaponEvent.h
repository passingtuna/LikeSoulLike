// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "LikeSoulLikeType.h"
#include "AN_WeaponEvent.generated.h"

/**
 * 
 */
UCLASS()
class LIKESOULLIKE_API UAN_WeaponEvent : public UAnimNotify
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EActionInputType InputType;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
