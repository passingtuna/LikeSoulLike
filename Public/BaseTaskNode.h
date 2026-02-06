// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BTService.h"
#include "LikeSoulLikeType.h"
#include "BaseTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class LIKESOULLIKE_API UReturnToSpawnLocation : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};

UCLASS()
class LIKESOULLIKE_API UActionTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	EActionInputType ActionInput;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};

UCLASS()
class LIKESOULLIKE_API UMoveToTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector KeySelector;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};


UENUM()
enum class ECompareOp : uint8
{
	Greater,
	Less,
	Equal,
	GreaterEqual,
	LessEqual,
};

UCLASS()
class LIKESOULLIKE_API UCompareThanDecorator : public UBTDecorator
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Compare")
	ECompareOp CompareOp;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector Compare;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	bool IsCheckConstant = false;
	UPROPERTY(EditAnywhere, Category = "Blackboard" , meta = (EditCondition = "IsCheckConstant == false", EditConditionHides))
	FBlackboardKeySelector Than;
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (EditCondition =  "IsCheckConstant == true", EditConditionHides))
	float Constant_Than;
	virtual bool CalculateRawConditionValue( UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory) const override;
};



UCLASS()
class LIKESOULLIKE_API UBTReturnToSpawnService : public UBTService
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BT")
	FBlackboardKeySelector TartgetKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BT")
	FBlackboardKeySelector IsReturnSpawnKey;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory,float DeltaSeconds) override;

};
