// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstanceDefaultBase.h"

void UAnimInstanceDefaultBase::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (IsTwoHandedWeapon)
    {
        CharacterMeshSocket = GetSkelMeshComponent()->GetSocketByName(FName("ElbowJointTarget"));
        if (CharacterMeshSocket)
        {
            ElbowLocation = GetSkelMeshComponent()->GetSocketLocation(CharacterMeshSocket->SocketName);
        }
        if (WeaponLeftHandGrip)
        {
            LeftHandGripLocation = WeaponLeftHandGrip->GetComponentLocation();
        }
    }

    if (AimOffsetMode)
    {
		FRotator tempController = OwnerCharacter->GetController()->GetControlRotation();
        FRotator tempActor  = OwnerCharacter->GetActorRotation();

        float tempAimYaw   =  FMath::FindDeltaAngleDegrees(tempActor.Yaw, tempController.Yaw);
        AimYaw   = FMath::Clamp(tempAimYaw, -89.9f, 89.9f);
        float tempAimPitch = FMath::UnwindDegrees(tempController.Pitch);
        AimPitch = FMath::Clamp(tempAimPitch, -89.9f, 89.9f);

        tempController.Pitch = AimPitch;
        tempController.Yaw = OwnerCharacter->GetActorRotation().Yaw + AimYaw;
        OwnerCharacter->GetController()->SetControlRotation(tempController);
  }

}