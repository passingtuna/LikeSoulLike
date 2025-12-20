// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterDefaultBase.h"
#include "WeaponDefaultBase.h"
#include "Engine/SkeletalMeshSocket.h"
#include "AnimInstanceDefaultBase.generated.h"

/**
 * 
 */
UCLASS()
class LIKESOULLIKE_API UAnimInstanceDefaultBase : public UAnimInstance
{
	GENERATED_BODY()
private:
	ACharacterDefaultBase* OwnerCharacter;
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsCrouch;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//기본 모션 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* WeaponIdleMotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* WeaponWalkMotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* WeaponRunMotion;

	//Two Bone IK 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsTwoHandedWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector	ElbowLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LeftHandGripLocation;
	USceneComponent* WeaponLeftHandGrip;
	const USkeletalMeshSocket* CharacterMeshSocket;

	//Aim Offset 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AimOffsetMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimPitch;




	void SetOwnerCharacter(ACharacterDefaultBase* NewOwnerCharacter)
	{
		OwnerCharacter = NewOwnerCharacter;
		CharacterMeshSocket = OwnerCharacter->GetMesh()->GetSocketByName("ElbowJointTarget");
	}

	void SetAimOffsetMode(bool NewAimOffsetMode)
	{
		AimOffsetMode = NewAimOffsetMode;
	}

	void SetIsTwoHandedWeapon(bool newIsTwoHandedWeapon)
	{
		IsTwoHandedWeapon = newIsTwoHandedWeapon;
	}
	void SetWeaponLeftHandGrip(USceneComponent * twoBoneIKTagetLocation )
	{
		WeaponLeftHandGrip = twoBoneIKTagetLocation;
	}
	
	void SetIsCrouch(bool NewIsCrouch)
	{
		IsCrouch = NewIsCrouch;
	}
};
