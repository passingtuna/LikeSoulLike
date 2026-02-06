// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_AIController.h"
#include "CharacterNonPlayableBase.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationPath.h" 
#include "CharacterPlayableBase.h"
#include "Perception/AIPerceptionComponent.h"


ETeamAttitude::Type ABase_AIController::GetTeamAttitudeTowards(const AActor & Other) const
{
    const IGenericTeamAgentInterface* OtherAgent = Cast<IGenericTeamAgentInterface>(&Other);
    if (!OtherAgent) return ETeamAttitude::Neutral;
    if(OtherAgent->GetGenericTeamId() == static_cast<uint8>(ETeam::T_Neutral))return ETeamAttitude::Neutral;
    return (OtherAgent->GetGenericTeamId() == GetGenericTeamId()) ? ETeamAttitude::Friendly : ETeamAttitude::Hostile;
}


ABase_AIController::ABase_AIController()
{
    // Blackboard Component 생성
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
    AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
    NpcActor = nullptr;
    TargetChar = nullptr;
    NavSys = nullptr;
    BlackboardAsset = nullptr;
    BehaviorTreeAsset = nullptr;
}

void ABase_AIController::BeginPlay()
{
    Super::BeginPlay();

    NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (AIPerceptionComp)
    {
        AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &ABase_AIController::UpdateTarget);
    }

}
void ABase_AIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    NpcActor = Cast<ACharacterNonPlayableBase>(InPawn);
    if (BlackboardAsset)
    {
        UseBlackboard(BlackboardAsset, BlackboardComp);
        BlackboardComp->SetValueAsFloat("Trace_AttackRange", 200);
    }
    if (BehaviorTreeAsset)
    {
        RunBehaviorTree(BehaviorTreeAsset);
    }
    SpawnLocation = NpcActor->GetActorLocation();
    SpawnRotator = NpcActor->GetActorRotation();
}

void ABase_AIController::Tick(float DeltaTime)
{
    if(NpcActor)
    {
        if (TargetChar)
        {
            BlackboardComp->SetValueAsFloat("Trace_FarFromTarget", FVector::Distance(TargetChar->GetActorLocation(), NpcActor->GetActorLocation()));
        }
    }
}


void ABase_AIController::SetTarget(AActor* Target)
{
    ACharacterPlayableBase* targetChar = Cast<ACharacterPlayableBase>(Target);
    TargetChar = targetChar;
    BlackboardComp->SetValueAsObject("Trace_Target", Target);
    NpcActor->TryLockOn(targetChar);
}
void ABase_AIController::UpdateTarget(AActor * Actor, FAIStimulus Stimulus)
{
    ACharacterPlayableBase* targetChar = Cast<ACharacterPlayableBase>(Actor);
    if (targetChar && Stimulus.WasSuccessfullySensed())
    {
        TargetChar = targetChar;
        BlackboardComp->SetValueAsObject("Trace_Target", Actor);
        NpcActor->TryLockOn(targetChar);
    }
    else
    {
        TargetChar = nullptr;
        BlackboardComp->SetValueAsObject("Trace_Target", nullptr);
        BlackboardComp->SetValueAsVector("Trace_TargetLastLocation", Actor->GetActorLocation());
        NpcActor->ReleaseLockOn();
        StopMovement();
    }
}

bool ABase_AIController::ReturnToSpawn()
{
    if (NpcActor->GetIsMoveable())
    {
        MoveToLocation(SpawnLocation, 20.f);
        NpcActor->ReleaseLockOn();
        if (FVector::Distance(SpawnLocation, NpcActor->GetActorLocation()) < 60)
        {
            NpcActor->SetActorRotation(SpawnRotator);
            StopMovement();
        }
    }
    return true;
}
void ABase_AIController::OnReturnCompletedCallback(FAIRequestID RequestID, const EPathFollowingResult::Type Result)
{
    ReceiveMoveCompleted.RemoveDynamic(this, &ABase_AIController::OnReturnCompletedCallback);
}

bool ABase_AIController::ExcuteMoveTo(UObject* object)
{
    if (NpcActor->GetIsMoveable() && !NpcActor->GetIsDead())
    {
        AActor* tempActor = Cast<AActor>(object);
        if (tempActor)
        {
            MoveToActor(tempActor);
            return true;
        }
    }
    return false;
}
bool ABase_AIController::ExcuteMoveTo(FVector vector)
{
    if (NpcActor->GetIsMoveable() && !NpcActor->GetIsDead())
    {
        FNavLocation Projected;
        if (NavSys && NavSys->ProjectPointToNavigation(vector, Projected))
        {
            MoveToLocation(Projected.Location, 20.f);
            return true;
        }
    }
    return false;
}

bool ABase_AIController::ExcuteAttack()
{
    if (NpcActor->GetIsAttackable())
    {
        NpcActor->ExcuteWeakAttack();
        return true;
    }
    return false;
}