// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_SwordSheild.h"
#include "CharacterDefaultBase.h"

void AWeapon_SwordSheild::SettingWeaponData(ACharacterDefaultBase * Character)
{
    Super::SettingWeaponData(Character);

    SwordMesh = mapWeaponMesh.Find("Sword")->Mesh;
}


void AWeapon_SwordSheild::ExcueteWeaponEvent(EActionInputType type)
{
    switch (EventNum)
    {
    case 1:
    {
        if (SwordMesh)
        {
            SwordMesh->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "RightHandSocket");
            SwordMesh->SetRelativeRotation(FRotator(0, 0, 90));
        }
    }
    break;
    case 2:
    {
        if (SwordMesh)
        {
            SwordMesh->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "LeftArmSocket");
            SwordMesh->SetRelativeLocation(FVector(0, 0, 30));
            SwordMesh->SetRelativeRotation(FRotator(0, 90, 90));
        }

    }
    break;

    case 3:
    {
        if (SwordMesh)
        {
            SwordMesh->SetRelativeLocation(FVector(0, 0, 60));
        }
    }
    break;

    case 4:
    {
        if (SwordMesh)
        {
            SwordMesh->SetRelativeLocation(FVector(0, 0, 0));
        }
    }
    break;
    case 5:
    {
        if (SwordMesh)
        {
            SwordMesh->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "RightHandSocket");
            SwordMesh->SetRelativeLocation(FVector(-7, 2, -3));
            SwordMesh->SetRelativeRotation(FRotator(0, 70, -90));
        }
    }
    break;
    default:
        EventNum = 0;
        break;
    }
}

void AWeapon_SwordSheild::StrongAttackProcess(ETriggerEvent Trigger)
{
    Super::StrongAttackProcess(Trigger);
}