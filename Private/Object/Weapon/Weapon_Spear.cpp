// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Spear.h"
#include "CharacterDefaultBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void AWeapon_Spear::AvoidProcess(ETriggerEvent Trigger)
{

    switch (Trigger)
    {
    case ETriggerEvent::Completed:
    {
        if (OwnerCharacter)
        {
            OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
            OwnerCharacter->LaunchCharacter(OwnerCharacter->GetLastInputDir() * 2000, false, false);
        }
    }
    break;
    default: break;
    }
}



void AWeapon_Spear::WeaponSkillProcess(ETriggerEvent Trigger)
{
    switch (Trigger)
    {
    case ETriggerEvent::Started:
    {
        if (OwnerCharacter)
        {
            FTimerHandle Handle;
            GetWorld()->GetTimerManager().SetTimer(Handle, [this]()
                {
                    OwnerCharacter->SetAnimMontagePlaySpeed(2.0f);


                    FTimerHandle Handle2;
                    GetWorld()->GetTimerManager().SetTimer(Handle2, [this]()
                        {
                            OwnerCharacter->SetAnimMontagePlaySpeed(1.0f);
                        },
                        10,
                        false
                    );
                },
                2,
                false
            );
        }
    }
    break;
    default: break;
    }
}


