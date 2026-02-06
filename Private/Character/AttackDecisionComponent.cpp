// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackDecisionComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Manager_Calculate.h"
#include "CharacterDefaultBase.h"
#include "Engine/OverlapResult.h"

// Sets default values for this component's properties
UAttackDecisionComponent::UAttackDecisionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
    Radius = 10.f;
	// ...
}


// Called when the game starts
void UAttackDecisionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
    TArray<USceneComponent*> Components;
    GetOwner()->GetComponents(Components);

    for (USceneComponent* Comp : Components)
    {
        if (Comp && Comp->GetName().StartsWith(TEXT("AttackDecisionLoc")))
        {
            FDecisionSweepData temp;
            temp.Point = Comp;
            temp.PrevPos = Comp->GetComponentLocation();
            arrDecisionPoint.Add(temp);
        }
    }
}

void UAttackDecisionComponent::StartAttack(FDamageData& tempdataInfo, ACharacterDefaultBase* attacker)
{
    IsAttacking = true;
    DataInfo = tempdataInfo;
    Attacker = attacker;
    HitActors.Empty();
    for (auto & Elem : arrDecisionPoint)
    {
        if (Elem.Point)
        {
            Elem.PrevPos = Elem.Point->GetComponentLocation();
        }
    }
    switch (DecisionShape)
    {
    case EDecisionShapeType::AST_Sphere:
        DecisionCollisionShape = FCollisionShape::MakeSphere(Radius);
        break;
    case EDecisionShapeType::AST_Capsule:
        DecisionCollisionShape = FCollisionShape::MakeCapsule(Radius, HalfHeight);
        break;
    case EDecisionShapeType::AST_Box:
        DecisionCollisionShape = FCollisionShape::MakeBox(BoxScale);
        break;
    default:
        break;
    }

 }

// Called every frame
void UAttackDecisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!IsAttacking)
        return;

    for (int32 i = 0 ;  i < arrDecisionPoint.Num() ; ++i)
    {
        if (!arrDecisionPoint[i].Point) continue;

        switch (DecisionType)
        {
        case EAttackDecisionType::ADT_Sweep:
            PerformSweep(i, DecisionCollisionShape);
            break;
        case EAttackDecisionType::ADT_AreaOnce:
            PerformAreaOnce(arrDecisionPoint[i].Point, DecisionCollisionShape);
            break;
        case EAttackDecisionType::ADT_AreaTick:
            DamageTick += DeltaTime;
            if (DamageTick >= TickInterval)
            {
                PerformAreaTick(arrDecisionPoint[i].Point, DecisionCollisionShape);
                DamageTick = 0;
            }
            break;
        default:
            break;
        }
    }
}

void UAttackDecisionComponent::PerformSweep(int32 Index, FCollisionShape Collision)
{
    if (!arrDecisionPoint.IsValidIndex(Index)) return;

    FCollisionQueryParams Params(SCENE_QUERY_STAT(WeaponSweep), false);
    Params.AddIgnoredActor(GetOwner());
    Params.AddIgnoredActor(Attacker);
    Params.bTraceComplex = false;
    Params.bReturnPhysicalMaterial = false;

    USceneComponent * DecisionLoc = arrDecisionPoint[Index].Point;

    FVector DecisionLocation = DecisionLoc->GetComponentLocation();
    FQuat DecisionQuat      = DecisionLoc->GetComponentQuat();
    FVector PrevSweepPos = arrDecisionPoint[Index].PrevPos;


    switch (DecisionShape)
    {
    case EDecisionShapeType::AST_Sphere:
    {
        DrawDebugSphere(GetWorld(), PrevSweepPos, Radius, 16, FColor::Red, false, 0.05f);
        DrawDebugSphere(GetWorld(), DecisionLocation, Radius, 16, FColor::Green, false, 0.05f);
        DrawDebugLine(GetWorld(), PrevSweepPos, DecisionLocation, FColor::Yellow, false, 0.05f, 0, 1.0f);
    }
    break;
    case EDecisionShapeType::AST_Capsule:
    {
        DrawDebugCapsule(GetWorld(), DecisionLocation, HalfHeight, Radius, DecisionQuat, FColor::Green, false, 0.05f);
    }
    break;
    case EDecisionShapeType::AST_Box:
    {
        DrawDebugBox(GetWorld(), DecisionLocation, BoxScale, DecisionQuat, FColor::Blue, false, 0.05f);
    }
    break;
    default:
        break;
    }

    TArray<FHitResult> HitResults;
    bool bHit = GetWorld()->SweepMultiByChannel(HitResults,PrevSweepPos,DecisionLocation,DecisionQuat,ECC_Pawn,Collision,Params);

    arrDecisionPoint[Index].PrevPos = DecisionLocation;

    if (!bHit)
        return;

    for (const FHitResult& Hit : HitResults)
    {
        AActor* HitActor = Hit.GetActor();
        if (!HitActor)
            continue;

        if (HitActors.Contains(HitActor)) continue;
        HitActors.Add(HitActor);
        OnHitSweep.ExecuteIfBound(Hit);

        ACharacterDefaultBase* TargetChar = Cast<ACharacterDefaultBase>(HitActor);
        if (TargetChar)
        {
            if (Attacker->CheckHostileChar(TargetChar, true))//공격은 중립도 당한다.
            {
                FVector HitDirection = -Hit.ImpactNormal;
                TargetChar->ReceiveAttack(Attacker, DataInfo, HitDirection);
            }
        }
    }
}

void UAttackDecisionComponent::EndAttack() 
{ 
    IsAttacking = false; 
};


void UAttackDecisionComponent::PerformAreaOnce(USceneComponent* DecisionLoc, FCollisionShape Collision)
{
    if (!DecisionLoc) return;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(WeaponSweep), false);
    Params.AddIgnoredActor(GetOwner());
    Params.AddIgnoredActor(Attacker);
    Params.bTraceComplex = false;
    Params.bReturnPhysicalMaterial = false;

    FVector DecisionLocation = DecisionLoc->GetComponentLocation();
    FQuat DecisionQuat = DecisionLoc->GetComponentQuat();

    DrawDebugSphere(GetWorld(), DecisionLocation, Radius, 24, FColor::Purple, false, TickInterval);

    TArray<FOverlapResult> HitResults;
    bool bHit = GetWorld()->OverlapMultiByChannel( HitResults, DecisionLocation, DecisionQuat, ECC_Pawn, Collision,Params);
    if (!bHit)
        return;

    for (const FOverlapResult& Hit : HitResults)
    {
        AActor* HitActor = Hit.GetActor();
        if (!HitActor)
            continue;

        if (HitActors.Contains(HitActor)) continue;
        HitActors.Add(HitActor);
        OnHitAreaOnce.ExecuteIfBound(Hit);

        ACharacterDefaultBase* TargetChar = Cast<ACharacterDefaultBase>(HitActor);
        if (TargetChar)
        {
            if (Attacker->CheckHostileChar(TargetChar, true))//공격은 중립도 당한다.
            {
                FVector HitDirection = HitActor->GetActorLocation() - DecisionLocation;
                TargetChar->ReceiveAttack(Attacker, DataInfo, HitDirection);
            }
        }
    }
    EndAttack(); //끝나고 자동으로 꺼준다
}

void UAttackDecisionComponent::PerformAreaTick(USceneComponent* DecisionLoc, FCollisionShape Collision)
{
    if (!DecisionLoc) return;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(WeaponSweep), false);
    Params.AddIgnoredActor(GetOwner());
    Params.AddIgnoredActor(Attacker);
    Params.bTraceComplex = false;
    Params.bReturnPhysicalMaterial = false;

    FVector DecisionLocation = DecisionLoc->GetComponentLocation();
    FQuat DecisionQuat = DecisionLoc->GetComponentQuat();

    DrawDebugSphere(GetWorld(), DecisionLocation, Radius, 24, FColor::Purple, false, TickInterval);
    TArray<FOverlapResult> HitResults;
    bool bHit = GetWorld()->OverlapMultiByChannel(HitResults, DecisionLocation, DecisionQuat, ECC_Pawn, Collision, Params);

    if (!bHit)
        return;

    for (const FOverlapResult& Hit : HitResults)
    {
        AActor* HitActor = Hit.GetActor();
        if (!HitActor)
            continue;

        if (HitActors.Contains(HitActor)) continue;
        HitActors.Add(HitActor);
        OnHitAreaOnce.ExecuteIfBound(Hit);

        ACharacterDefaultBase* TargetChar = Cast<ACharacterDefaultBase>(HitActor);
        if (TargetChar)
        {

            if (Attacker->CheckHostileChar(TargetChar, true))//공격은 중립도 당한다.
            {
                FVector HitDirection = HitActor->GetActorLocation() - DecisionLocation;
                TargetChar->ReceiveAttack(Attacker, DataInfo, HitDirection);
            }
        }
    }
}

void UAttackDecisionComponent::PerformAreaOnceAtLocation(FVector DecisionLoc, float radius)
{
    FCollisionQueryParams Params(SCENE_QUERY_STAT(WeaponSweep), false);
    Params.AddIgnoredActor(GetOwner());
    Params.AddIgnoredActor(Attacker);
    Params.bTraceComplex = false;
    Params.bReturnPhysicalMaterial = false;

    FVector DecisionLocation = DecisionLoc;
    FCollisionShape Collision = FCollisionShape::MakeSphere(radius);

    DrawDebugSphere(GetWorld(), DecisionLocation, radius, 24, FColor::Purple, false, TickInterval);

    TArray<FOverlapResult> HitResults;
    bool bHit = GetWorld()->OverlapMultiByChannel(HitResults, DecisionLocation, FQuat::Identity, ECC_Pawn, Collision, Params);
    if (!bHit)
        return;

    for (const FOverlapResult& Hit : HitResults)
    {
        AActor* HitActor = Hit.GetActor();
        if (!HitActor)
            continue;

        if (HitActors.Contains(HitActor)) continue;
        HitActors.Add(HitActor);
        OnHitAreaOnce.ExecuteIfBound(Hit);

        ACharacterDefaultBase* TargetChar = Cast<ACharacterDefaultBase>(HitActor);
        if (TargetChar)
        {
            if (Attacker->CheckHostileChar(TargetChar, true))//공격은 중립도 당한다.
            {
                FVector HitDirection = HitActor->GetActorLocation() - DecisionLocation;
                TargetChar->ReceiveAttack(Attacker, DataInfo, HitDirection);
            }
        }
    }
}