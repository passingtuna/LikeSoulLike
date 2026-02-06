// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTaskNode.h"
#include "Base_AIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UReturnToSpawnLocation::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
    ABase_AIController* AICon = Cast<ABase_AIController>(OwnerComp.GetAIOwner());
    if (!AICon) return EBTNodeResult::Failed;

    if (AICon->ReturnToSpawn())
    {
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}

EBTNodeResult::Type UMoveToTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    if (!BB) return EBTNodeResult::Failed;
    ABase_AIController* AICon = Cast<ABase_AIController>(OwnerComp.GetAIOwner());
    if (!AICon) return EBTNodeResult::Failed;


    UObject * object = BB->GetValueAsObject(KeySelector.SelectedKeyName);
    FVector Vector = BB->GetValueAsVector(KeySelector.SelectedKeyName);
    
    if (object && AICon->ExcuteMoveTo(object)) return EBTNodeResult::Succeeded;
    else if(AICon->ExcuteMoveTo(Vector)) return EBTNodeResult::Succeeded;

    return EBTNodeResult::Failed;
}

EBTNodeResult::Type UActionTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ABase_AIController* AICon = Cast<ABase_AIController>(OwnerComp.GetAIOwner());
    if (!AICon) return EBTNodeResult::Failed;

    switch (ActionInput)
    {
        case EActionInputType::AIT_WeakAttack:
        {
            if (AICon->ExcuteAttack())
            {
                return EBTNodeResult::Succeeded;
            }
        }
        break;
        case EActionInputType::AIT_StrongAttack:
        {
            if (AICon->ExcuteAttack())
            {
                return EBTNodeResult::Succeeded;
            }
        }
        break;
        case EActionInputType::AIT_WeaponAction:
        {
            if (AICon->ExcuteAttack())
            {
                return EBTNodeResult::Succeeded;
            }
        }
        break;
        case EActionInputType::AIT_WeaponSkill:
        {
            if (AICon->ExcuteAttack())
            {
                return EBTNodeResult::Succeeded;
            }
        }
        break;
    }
    return EBTNodeResult::Failed;
}


bool UCompareThanDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    if (!BB) return false;
    float A = BB->GetValueAsFloat(Compare.SelectedKeyName);
    float B = BB->GetValueAsFloat(Than.SelectedKeyName);

    if (IsCheckConstant)
    {
        B = Constant_Than;
    }

    switch (CompareOp)
    {
        case ECompareOp::Greater: return A > B;
        case ECompareOp::Less:    return A < B;
        case ECompareOp::Equal:   return FMath::IsNearlyEqual(A, B);
        case ECompareOp::GreaterEqual:return FMath::IsNearlyEqual(A, B) || A > B;
        case ECompareOp::LessEqual:   return FMath::IsNearlyEqual(A, B) || A < B;
        default: return false;
    }
    
}
void UBTReturnToSpawnService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    ABase_AIController* AICon = Cast<ABase_AIController>(OwnerComp.GetAIOwner());
    if (!AICon) return;
    APawn* AIPawn = AICon->GetPawn();
    if (!AIPawn) return;
    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    if (!BB) return;
    FVector SpawnLocation = AICon->SpawnLocation;
    bool bIsTargetValid = true; 
    UObject * Target = BB->GetValueAsObject(TartgetKey.SelectedKeyName);
    if (!Target) bIsTargetValid = false;

    float DistanceFromSpawn = FVector::Dist(AIPawn->GetActorLocation(), SpawnLocation);
    bool bReturn = !bIsTargetValid || (DistanceFromSpawn > 1500);
    if (DistanceFromSpawn < 60) bReturn = false;
    BB->SetValueAsBool(IsReturnSpawnKey.SelectedKeyName, bReturn);
}