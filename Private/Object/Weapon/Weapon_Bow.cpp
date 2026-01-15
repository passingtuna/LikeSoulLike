// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Bow.h"
#include "CharacterDefaultBase.h"
#include "CharacterPlayableBase.h"
#include "AnimInstanceDefaultBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "ControlRig.h"
#include "ControlRigComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstanceBow.h"
#include "Projectile_Default.h"
#include "DA_WeaponDefaultData.h"
#include "Manager_Calculate.h"
#include "Manager_Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

void AWeapon_Bow::BeginPlay()
{
    Super::BeginPlay();
    ProjectileManager = GetWorld()->GetGameInstance()->GetSubsystem<UManager_Projectile>();

    StringMidLoc = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("BowStringMid")));
    ArrowPointLoc = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("ArrowPointPos")));
    LuanchPos = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("LauchPos")));
    SetActorEnableCollision(false);
    
    MultiShot = false;
    IsAiming = false;
    IsLoadingBow = false;
}

void AWeapon_Bow::SettingWeaponData(ACharacterDefaultBase* Character)
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
                    if (Elem.MeshName == "Bow") BowMesh = Cast<USkeletalMeshComponent>(Comp);
                    if (Elem.MeshName == "MainArrow") ArrowMesh = Cast<UStaticMeshComponent>(Comp);
                    if (Elem.MeshName == "LineMesh") TrajectoryMesh = Cast<UStaticMesh>(Comp);
                    if (Elem.MeshName.Left(5) == "Arrow") AdditionalArrow.Add(Cast<UStaticMeshComponent>(Comp));

                    Comp->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, Elem.SocketName);
                    Comp->SetRelativeTransform_Direct(Elem.Transform);
                    Comp->SetVisibility(Elem.IsVisible);
                }
            }
        }
    }

    SplineComp = GetComponentByClass<USplineComponent>();
    CalculateBaseDamage(OwnerCharacter);
    OwnerCharacter = Character;
    if (BowMesh)
    {
        BowAnimInstance = Cast<UAnimInstanceBow>(BowMesh->GetAnimInstance());
        BowAnimInstance->SetBowStringTransform(StringMidLoc->GetRelativeTransform());
    }
}


void AWeapon_Bow::ExcueteWeaponEvent(EActionInputType type)
{
    switch (EventNum)
    {
        case 1:
        {
            ArrowLoaded();
        }
        break;
        case 2:
        {
            ArrowFired();
        }
        break;
    }
}

void AWeapon_Bow::Tick(float DeltaTime)
{
    if (IsLoadingBow)
    {
        FTransform SocketTransform = OwnerCharacter->GetMesh()->GetSocketTransform(TEXT("BowFingerSocket"), RTS_World);
        StringMidLoc->SetWorldTransform(SocketTransform);

        FVector StringLoc = StringMidLoc->GetComponentLocation();
        FVector ArrowLoc = ArrowPointLoc->GetComponentLocation();
        FVector CalDir = (ArrowLoc - StringLoc).GetSafeNormal();

        ArrowMesh->AttachToComponent(StringMidLoc, FAttachmentTransformRules::KeepRelativeTransform);
        ArrowMesh->SetRelativeLocation(FVector(0, 0, 0));
        FRotator ArrowRot = CalDir.Rotation();
        ArrowRot.Pitch = ArrowRot.Pitch - 90;
        ArrowMesh->SetWorldRotation(ArrowRot);

        if (MultiShot)
        {
            FRotator tempRot;
            for (int32 i = 0 ; i < AdditionalArrow.Num(); ++i)
            {
                FVector FanAxis = BowMesh->GetForwardVector();
                float AngleDeg;
                if (i < 3)
                {
                    AngleDeg = -30 + (i * 10.f);
                }
                else
                {
                    AngleDeg = (i - 2) * 10.f;
                }
                FVector RotatedDir = CalDir.RotateAngleAxis(AngleDeg, FanAxis);
                FRotator ArrowRot1 = RotatedDir.Rotation();

                ArrowRot1.Pitch -= 90.f;
                
                AdditionalArrow[i]->SetWorldRotation(ArrowRot1);
                AdditionalArrow[i]->AttachToComponent(StringMidLoc,FAttachmentTransformRules::KeepWorldTransform);
                AdditionalArrow[i]->SetRelativeLocation(FVector(0, 0, 0));
            }
        }
        BowAnimInstance->SetBowStringTransform(StringMidLoc->GetRelativeTransform());
    }
}

void AWeapon_Bow::ExcuteWeakAttackEvent() 
{
}  

void AWeapon_Bow::ArrowLoaded()
{
    IsLoadingBow = true;
    ArrowMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
    ArrowMesh->AttachToComponent(StringMidLoc, FAttachmentTransformRules::KeepRelativeTransform);
    ArrowMesh->SetRelativeLocation(FVector(0, 0, 0));
}
void AWeapon_Bow::ArrowFired()
{
    IsLoadingBow = false;
    FDamageData FinalDamage = CalBaseDamage * CurrentMotionMutiply;
    if(IsAiming)
    {
        AController* pc = OwnerCharacter->GetController();
        FVector FireDir;
        FRotator AimRot;
        if (pc && pc->GetPawn() == OwnerCharacter) //조준모드는 플레이어만..?
        {
            AimRot = pc->GetControlRotation();
            FireDir = AimRot.Vector();
          
            if (MultiShot)
            {
                for (int32 i = 0; i < 7; ++i)
                {
                    float YawOffset = (i - 3) * 15;
                    FRotator ShotRot = AimRot;
                    ShotRot.Yaw += YawOffset;
                    FVector ShotDir = ShotRot.Vector();
                    AProjectile_Default* Arrow = ProjectileManager->GetProjectile("NormalArrow");//GetWorld()->SpawnActor<AProjectile_Default>(ArrowType,LuanchPos->GetComponentLocation(),ShotDir.Rotation());

                    if(Arrow)
                    {
                        Arrow->SetActorLocation(LuanchPos->GetComponentLocation());
                        Arrow->SetActorRotation(ShotDir.Rotation());
                        Arrow->Fire(OwnerCharacter, FinalDamage, LuanchPos->GetComponentLocation(), ShotDir, 3000);
                    }
                }
            }
            else
            {
                AProjectile_Default* Arrow = ProjectileManager->GetProjectile("NormalArrow");
                if (Arrow)
                {
                    Arrow->SetActorLocation(LuanchPos->GetComponentLocation());
                    Arrow->SetActorRotation(FireDir.Rotation());
                    Arrow->Fire(OwnerCharacter, FinalDamage, LuanchPos->GetComponentLocation(), FireDir, 3000);
                }
            }   
        }
    }
    else
    {
        if (MultiShot)
        {
            for (int32 i = 0; i < 7; ++i)
            {
                FVector BaseDir = OwnerCharacter->GetActorForwardVector();
                float SpreadAngle = -45.f + (i * 15.f);
                FVector ShotDir = FRotator(0.f, SpreadAngle, 0.f).RotateVector(BaseDir);

                AProjectile_Default* Arrow = ProjectileManager->GetProjectile("ExplosionArrow");
                if (Arrow) //AProjectile_Default* Arrow = GetWorld()->SpawnActor<AProjectile_Default>(ArrowType, LuanchPos->GetComponentLocation(), BaseDir.Rotation());
                {
                    Arrow->SetActorLocation(LuanchPos->GetComponentLocation());
                    Arrow->SetActorRotation(ShotDir.Rotation());
                    Arrow->Fire(OwnerCharacter, FinalDamage, LuanchPos->GetComponentLocation(), ShotDir, 3000);
                }
            }
        }
        else
        {
            FVector BaseDir = OwnerCharacter->GetActorForwardVector();
            AProjectile_Default* Arrow = ProjectileManager->GetProjectile("ExplosionArrow");
            //AProjectile_Default* Arrow = GetWorld()->SpawnActor<AProjectile_Default>(ArrowType, LuanchPos->GetComponentLocation(), BaseDir.Rotation());
            if (Arrow)
            {
                Arrow->SetActorLocation(LuanchPos->GetComponentLocation());
                Arrow->SetActorRotation(BaseDir.Rotation());
                Arrow->Fire(OwnerCharacter, FinalDamage, LuanchPos->GetComponentLocation(), BaseDir, 3000);
            }
        }
    }
    

    MultiShot = false;
    ResetWeaponEvent();
    StringMidLoc->SetRelativeLocation(FVector(0, -14.5, 0));
    BowAnimInstance->SetBowStringTransform(StringMidLoc->GetRelativeTransform());

}

void AWeapon_Bow::StrongAttackProcess(ETriggerEvent Trigger)
{
}
void AWeapon_Bow::WeaponSkillProcess(ETriggerEvent Trigger)
{
    switch (Trigger)
    {
    case ETriggerEvent::Started:
        MultiShot = true;
        break;
    }
}
void AWeapon_Bow::CalculatePredictProjectile()
{
    if (!IsAiming || !OwnerCharacter || !LuanchPos) return;

    if (!SplineComp) return;

    FVector FireDir;
    AController* PC = OwnerCharacter->GetController();
    if (PC && PC->GetPawn() == OwnerCharacter)
    {
        FireDir = PC->GetControlRotation().Vector();
    }
    else
    {
        FireDir = OwnerCharacter->GetActorForwardVector();
    }

    FPredictProjectilePathParams Params;
    Params.StartLocation = LuanchPos->GetComponentLocation();
    Params.LaunchVelocity = FireDir * 3000.f;
    Params.bTraceWithCollision = true;
    Params.ProjectileRadius = TrajectoryRadius;
    Params.TraceChannel = ECC_Visibility;
    Params.MaxSimTime = TrajectorySimTime;
    Params.SimFrequency = 15.f;
    Params.ActorsToIgnore.Add(OwnerCharacter);

    FPredictProjectilePathResult Result;

    if (!UGameplayStatics::PredictProjectilePath(GetWorld(), Params, Result))
        return;

    const TArray<FPredictProjectilePathPointData>& Points = Result.PathData;
    if (Points.Num() < 2) return;
    SplineComp->ClearSplinePoints();
    for (int32 i = 0; i < Points.Num(); ++i)
    {
        SplineComp->AddSplinePoint(
            Points[i].Location,
            ESplineCoordinateSpace::World,
            false
        );
    }
    SplineComp->UpdateSpline();

    int32 SegmentCount = Points.Num() - 1;

    while (SplineMeshThrows.Num() < SegmentCount)
    {
        USplineMeshComponent* Mesh = NewObject<USplineMeshComponent>();
        Mesh->RegisterComponent();
        Mesh->SetStaticMesh(TrajectoryMesh);
        Mesh->SetMaterial(0, TrajectoryMaterial); 
        Mesh->SetMobility(EComponentMobility::Movable);
        Mesh->AttachToComponent(SplineComp, FAttachmentTransformRules::KeepRelativeTransform);
        SplineMeshThrows.Add(Mesh);
    }

    for (int32 i = 0; i < SplineMeshThrows.Num(); ++i)
    {
        if (i >= SegmentCount)
        {
            SplineMeshThrows[i]->SetVisibility(false);
            continue;
        }

        FVector StartPos, StartTangent;
        FVector EndPos, EndTangent;

        SplineComp->GetLocationAndTangentAtSplinePoint(
            i, StartPos, StartTangent, ESplineCoordinateSpace::World
        );
        SplineComp->GetLocationAndTangentAtSplinePoint(
            i + 1, EndPos, EndTangent, ESplineCoordinateSpace::World
        );

        SplineMeshThrows[i]->SetStartAndEnd(
            StartPos, StartTangent,
            EndPos, EndTangent
        );
        SplineMeshThrows[i]->SetVisibility(true);
    }
}

void AWeapon_Bow::WeaponActionProcess( ETriggerEvent Trigger)
{

    ACharacterPlayableBase* pc = Cast<ACharacterPlayableBase>(OwnerCharacter);
    if (!pc) return;//조준은 플레이어 캐릭터만
    switch (Trigger)
    {
        case ETriggerEvent::Started:
            pc->SetAimingMode(true);
            //GetWorld()->GetTimerManager().SetTimer(AimDrawTimer, this, &AWeapon_Bow::CalculatePredictProjectile, 0.05f, true);
            IsAiming = true;
        break;
        case ETriggerEvent::Completed:

            pc->SetAimingMode(false);
            GetWorld()->GetTimerManager().ClearTimer(AimDrawTimer);
            IsAiming = false;
            /*
            if (OwnerCharacter)
            {
                if (SplineComp)
                {
                    SplineComp->ClearSplinePoints();
                }
            }

            for (USplineMeshComponent* Mesh : SplineMeshThrows)
            {
                if (Mesh) Mesh->SetVisibility(false);
            }
            break;*/
    }
}

void AWeapon_Bow::AvoidProcess(ETriggerEvent Trigger)
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
