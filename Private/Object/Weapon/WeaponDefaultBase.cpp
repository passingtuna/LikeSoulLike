// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponDefaultBase.h"
#include "DA_ActionData.h"
#include "CharacterDefaultBase.h"
#include "CharacterPlayableBase.h"
#include "CharacterNonPlayableBase.h"
#include "AnimInstanceDefaultBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DA_WeaponDefaultData.h"
#include "AttackDecisionComponent.h"
#include "Manager_Calculate.h"
#include "NiagaraComponent.h"
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
    CalculateManager = GetWorld()->GetGameInstance()->GetSubsystem<UManager_Calculate>();
    AttackDecisionComp = GetComponentByClass<UAttackDecisionComponent>();

}

// Called every frame
void AWeaponDefaultBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AWeaponDefaultBase::SettingWeaponData(ACharacterDefaultBase* Character)
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
                    FWeaponMeshInitData temp;
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


    TArray<UNiagaraComponent*> Components;
    GetComponents<UNiagaraComponent>(Components);

    for (UNiagaraComponent* Comp : Components)
    {
        if (Comp)
        {
            Comp->DeactivateImmediate();
            mapNiagara.Add(FName(Comp->GetName()), Comp);
        }
    }
    

    OwnerCharacter = Character;
    UE_LOG(LogTemp, Log, TEXT("1"));
    CalculateBaseDamage(OwnerCharacter);
    UE_LOG(LogTemp, Log, TEXT("2"));
    SetActorEnableCollision(false);
    UE_LOG(LogTemp, Log, TEXT("3"));
    if (bIsTwoHanded)
    {
        LeftHandGripComponet = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("LeftHandGrip")));
    }
}
void AWeaponDefaultBase::CalculateBaseDamage(ACharacterDefaultBase* Character)
{
    UE_LOG(LogTemp, Log, TEXT("4"));
    ACharacterPlayableBase* player = Cast<ACharacterPlayableBase>(Character);
    if(IsValid(player))
    {
        UE_LOG(LogTemp, Log, TEXT("5"));
        int32 Upgrade = 0;
        EWeaponInfusionType Infusion = EWeaponInfusionType::EWIT_None;
        if (Itemdata)
        {
            Upgrade = Itemdata->Upgrade;
            Infusion = Itemdata->Infusion;
        }
        CalBaseDamage = CalculateManager->GetWeaponBaseDamage(this, Upgrade, Infusion, player->GetCurrentStatus());
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("6"));
        ACharacterNonPlayableBase* Nplayer = Cast<ACharacterNonPlayableBase>(Character);

        CalBaseDamage = DADefaultData->DamageData.BaseDamage;
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



void AWeaponDefaultBase::StrongAttackProcess(ETriggerEvent Trigger)
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
void AWeaponDefaultBase::WeakAttackProcess( ETriggerEvent Trigger)
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
void AWeaponDefaultBase::WeaponSkillProcess( ETriggerEvent Trigger)
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
void AWeaponDefaultBase::WeaponActionProcess( ETriggerEvent Trigger)
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
void AWeaponDefaultBase::AvoidProcess(ETriggerEvent Trigger)
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
void AWeaponDefaultBase::DecisionAttack(bool IsAttacking ,ACharacterDefaultBase* Attacker, UDA_ActionData* Actiondata)
{
    if (AttackDecisionComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("현재 모션 배율 : %f"), CurrentMotionMutiply);
        FDamageData FinalDamage = CalBaseDamage * CurrentMotionMutiply;
        FinalDamage = FinalDamage + BuffDamage;

        UE_LOG(LogTemp, Warning, TEXT("물리공격 데미지 : %f"), FinalDamage.PhysicalDamage);

        if (Actiondata)
        {
            FinalDamage.IsGuardable = Actiondata->IsGuardable;
            FinalDamage.IsParryable = Actiondata->IsParryable;
            FinalDamage.IsAvoidable = Actiondata->IsAvoidable;
        }

        IsAttacking ? AttackDecisionComp->StartAttack(FinalDamage, Attacker) : AttackDecisionComp->EndAttack();
    }
}



void AWeaponDefaultBase::OnWeaponBuff(FItemAffectData& AffectData)
{
    switch (AffectData.AffectType)
    {
        case EAffectionType::AT_FireDamage:
        {
            GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
            NowBuffName = "Fire";
            UNiagaraComponent** Comp = mapNiagara.Find(NowBuffName);
            if (Comp && *Comp)
            {
                (*Comp)->Activate();
            }
            BuffDamage.FireDamage = AffectData.Value;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AWeaponDefaultBase::OffWeaponBuff, AffectData.Duration, false);
        }
        break;
        case EAffectionType::AT_DevineDamage:
        {
            GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
            NowBuffName = "Devine";
            UNiagaraComponent** Comp = mapNiagara.Find(NowBuffName);
            if (Comp && *Comp)
            {
                (*Comp)->Activate();
            }
            BuffDamage.DevineDamage = AffectData.Value;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AWeaponDefaultBase::OffWeaponBuff, AffectData.Duration, false);
        }
        break;
        case EAffectionType::AT_MagicDamage:
        {
            GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
            NowBuffName = "Magic";
            UNiagaraComponent** Comp = mapNiagara.Find(NowBuffName);
            if (Comp && *Comp)
            {
                (*Comp)->Activate();
            }
            BuffDamage.MagicDamage = AffectData.Value;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AWeaponDefaultBase::OffWeaponBuff, AffectData.Duration, false);
        }
        break;
        case EAffectionType::AT_LightningDamage:
        {
            GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
            NowBuffName = "Lightning";
            UNiagaraComponent** Comp = mapNiagara.Find(NowBuffName);
            if (Comp && *Comp)
            {
                (*Comp)->Activate();
            }
            BuffDamage.LightningDamage = AffectData.Value;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AWeaponDefaultBase::OffWeaponBuff, AffectData.Duration, false);
        }
        break;
    }
}

void AWeaponDefaultBase::OffWeaponBuff()
{
    for (auto& Elem : mapNiagara)
    {
        Elem.Value->DeactivateImmediate();
    }
    BuffDamage = 0;
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}