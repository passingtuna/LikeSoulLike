// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponDefaultBase.h"
#include "DA_ActionData.h"
#include "CharacterDefaultBase.h"
#include "AnimInstanceDefaultBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DA_WeaponDefaultData.h"
#include "AttackDecisionComponent.h"
#include "Manager_Calculate.h"
// Sets default values
AWeaponDefaultBase::AWeaponDefaultBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWeaponDefaultBase::BeginPlay()
{
	Super::BeginPlay();
    EventNum = 0;
    CalculatManager = GetWorld()->GetGameInstance()->GetSubsystem<UManager_Calculate>();

    InfusionType = EWeaponInfusionType::EWIT_None;
}

// Called every frame
void AWeaponDefaultBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AWeaponDefaultBase::SettingWeaponLocation(ACharacterDefaultBase* Character)
{
    if (DADefaultData)
    {
        for (auto& Elem : DADefaultData->WeaponAttachData)
        {
            TArray<UPrimitiveComponent*> Components;
            GetComponents<UPrimitiveComponent>(Components);

            for (UPrimitiveComponent* Comp : Components)
            {
                if (Comp && Comp->GetName() == Elem.MeshName)
                {
                    WeaponMeshInitData temp;
                    temp.SocketName = Elem.SocketName;
                    temp.Mesh = Comp;
                    temp.InitTransForm = Elem.Transform;
                    temp.IsVisibility = Elem.IsVisible;

                    mapWeaponMesh.Add(FName(Elem.MeshName), temp);
                    Comp->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, Elem.SocketName);
                    Comp->SetRelativeTransform_Direct(Elem.Transform);
                    Comp->SetVisibility(Elem.IsVisible);
                }
            }
        }
    }


    AttackDecisionComp = GetComponentByClass<UAttackDecisionComponent>();

    OwnerCharacter = Character;
    SetActorEnableCollision(false);
    if (bIsTwoHanded)
    {
        LeftHandGripComponet = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("LeftHandGrip")));
    }
}

void AWeaponDefaultBase::SetActiveWeapon(bool bIsActive)
{
    if (!bIsActive)
    {
        SetActorHiddenInGame(true);
        SetActorTickEnabled(false);
        TArray<UActorComponent*> Components;
        GetComponents(Components);
        for (auto* Comp : Components)
        {
            if (auto* Prim = Cast<UPrimitiveComponent>(Comp))
            {
                Prim->SetCollisionEnabled(ECollisionEnabled::NoCollision);
                Prim->SetGenerateOverlapEvents(false);
            }
        }
    }
    else
    {
        SetActorHiddenInGame(false);
        SetActorTickEnabled(true);
        TArray<UActorComponent*> Components;
        GetComponents(Components);


        if (bIsTwoHanded)
        {
            if (LeftHandGripComponet)
            {
                OwnerCharacter->GetAnimInstance()->SetWeaponLeftHandGrip(LeftHandGripComponet);
            }
        }

        for (auto* Comp : Components)
        {
            if (auto* Prim = Cast<UPrimitiveComponent>(Comp))
            {
                Prim->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
                Prim->SetGenerateOverlapEvents(true);
            }
		}
    }
}



void AWeaponDefaultBase::StrongAttackProcess(ETriggerEvent Trigger, const FInputActionInstance& instance)
{
    switch (Trigger)
    {
    case ETriggerEvent::Started:
    {
        CurrentMotionMutiply = StrongAttackMotion->MotionDamageMutiply;
    }
    break;
    default: break;
    }
}
void AWeaponDefaultBase::WeakAttackProcess( ETriggerEvent Trigger, const FInputActionInstance& instance)
{
    switch (Trigger)
    {
    case ETriggerEvent::Started:
    {
        CurrentMotionMutiply = WeakAttackMotion->MotionDamageMutiply;
    }
    break;
    default: break;
    }
}
void AWeaponDefaultBase::WeaponSkillProcess( ETriggerEvent Trigger, const FInputActionInstance& instance)
{
    switch (Trigger)
    {
    case ETriggerEvent::Started:
    {
        CurrentMotionMutiply = WeaponSkillMotion->MotionDamageMutiply;
    }
    break;;
    default: break;
    }
}
void AWeaponDefaultBase::WeaponActionProcess( ETriggerEvent Trigger, const FInputActionInstance& instance)
{
    switch (Trigger)
    {
    case ETriggerEvent::Started:
    {
        CurrentMotionMutiply = WeaponActionMotion->MotionDamageMutiply;
    }
    break;
    default: break;
    }
}
void AWeaponDefaultBase::AvoidProcess( ETriggerEvent Trigger, const FInputActionInstance& instance)
{
    switch (Trigger)
    {
        case ETriggerEvent::Completed:
        {
            if (OwnerCharacter)
            {
                OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
                OwnerCharacter->LaunchCharacter(OwnerCharacter->GetLastInputDir() * 1000, false, false);
            }
        }
        break;
    default: break;
    }
}

void AWeaponDefaultBase::IncreaseWeaponEventNum(EActionInputType type)
{
    EventNum++;
    ExcueteWeaponEvent(type);
}

void AWeaponDefaultBase::ExcueteWeaponEvent(EActionInputType type)
{
}

void AWeaponDefaultBase::ResetWeaponEvent()
{
    
    if (EventNum != 0)
    {
        for (auto& Elem : mapWeaponMesh)
        {
            Elem.Value.Mesh->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, Elem.Value.SocketName);
            Elem.Value.Mesh->SetRelativeTransform_Direct(Elem.Value.InitTransForm);
        }
		EventNum = 0;
    }
}
void AWeaponDefaultBase::SetIsAttacking(bool IsAttacking , FDamageData*DataInfo , ACharacterDefaultBase* Attacker)
{
    if (AttackDecisionComp)
    {
        IsAttacking ? AttackDecisionComp->StartAttack(*DataInfo , Attacker) : AttackDecisionComp->EndAttack();
    }
}