// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_Inputable.generated.h"

/**
 * 
 */
UCLASS()
class LIKESOULLIKE_API UAN_Inputable : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsEnd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsMoveable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAvoidable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAttackable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsRunable;

	
};
