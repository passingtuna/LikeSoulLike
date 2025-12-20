// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_SwordSheild.h"
#include "CharacterDefaultBase.h"


void AWeapon_SwordSheild::ExcueteWeaponEvent(EActionInputType type)
{

    UPrimitiveComponent* tempComp;
    tempComp = mapWeaponMesh.Find("Sword")->Mesh;
    switch (EventNum)
    {
    case 1:
    {
        if (tempComp)
        {
            (tempComp)->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "RightHandSocket");
            (tempComp)->SetRelativeRotation(FRotator(0, 0, 90));
        }
    }
    break;
    case 2:
    {
        if (tempComp)
        {
            (tempComp)->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "LeftArmSocket");
            (tempComp)->SetRelativeLocation(FVector(0, 0, 30));
            (tempComp)->SetRelativeRotation(FRotator(0, 90, 90));
        }

    }
    break;

    case 3:
    {
        if (tempComp)
        {
            (tempComp)->SetRelativeLocation(FVector(0, 0, 60));
        }
    }
    break;

    case 4:
    {
        if (tempComp)
        {
            (tempComp)->SetRelativeLocation(FVector(0, 0, 0));
        }
    }
    break;
    case 5:
    {
        if (tempComp)
        {
            (tempComp)->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "RightHandSocket");
            (tempComp)->SetRelativeLocation(FVector(-7, 2, -3));
            (tempComp)->SetRelativeRotation(FRotator(0, 70, -90));
        }
    }
    break;
    default:
        EventNum = 0;
        break;
    }
}

void AWeapon_SwordSheild::StrongAttackProcess(ETriggerEvent Trigger, const FInputActionInstance& instance)
{
    Super::StrongAttackProcess(Trigger, instance);
    int MaxChargeStep = 3;

    switch (Trigger)
    {
    case ETriggerEvent::Started:
        ChargeStep = 0;
        break;
    case ETriggerEvent::Triggered:

        if (ChargeStep < MaxChargeStep)
            if (instance.GetElapsedTime() / 2.0f >= ChargeStep + 1)
            {
                CurrentMotionMutiply += 1;
                ChargeStep++;
            }
        break;
    case ETriggerEvent::Completed:
        break;
    }
}