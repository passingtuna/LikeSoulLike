// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LikeSoulLikeType.h"
#include "Delegates/Delegate.h"
#include "AttackDecisionComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnHitSphereSweep, const FHitResult&);
DECLARE_DELEGATE_OneParam(FOnHitIAreaOnce, const FOverlapResult&);
DECLARE_DELEGATE_OneParam(FOnHitAreaTick, const FOverlapResult&);

struct FDecisionSweepData
{
	USceneComponent* Point;
	FVector PrevPos;
};

class ACharacterDefaultBase;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIKESOULLIKE_API UAttackDecisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	FOnHitSphereSweep OnHitSweep;
	FOnHitIAreaOnce OnHitAreaOnce;
	FOnHitAreaTick	OnHitAreaTick;

	// Sets default values for this component's properties
	UAttackDecisionComponent();
	FDamageData DataInfo;
	ACharacterDefaultBase* Attacker;
	void StartAttack(FDamageData& tempdataInfo, ACharacterDefaultBase* attacker);
	void EndAttack();

	TArray<FDecisionSweepData> arrDecisionPoint;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
private:
	bool IsAttacking = false;
	TSet<AActor*> HitActors;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	EDecisionShapeType DecisionShape;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Attack")
	EAttackDecisionType DecisionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Attack" , meta = (EditCondition = "DecisionShape == EDecisionShapeType::AST_Sphere || DecisionShape == EDecisionShapeType::AST_Capsule",EditConditionHides))
	float Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (EditCondition = "DecisionShape == EDecisionShapeType::AST_Capsule", EditConditionHides))
	float HalfHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (EditCondition = "DecisionShape == EDecisionShapeType::AST_Box", EditConditionHides))
	FVector BoxScale;

	UPROPERTY(EditAnywhere, Category = "Attack", meta = (EditCondition = "DecisionType == EAttackDecisionType::ADT_AreaTick",EditConditionHides))
	float TickInterval = 0.5f;
	float DamageTick;

	FCollisionShape DecisionCollisionShape;

	void PerformSweep(int32 Index, FCollisionShape Collision);
	void PerformAreaOnce(USceneComponent* DecisionLoc, FCollisionShape Collision);
	void PerformAreaTick(USceneComponent* DecisionLoc, FCollisionShape Collision);

	void PerformAreaOnceAtLocation(FVector DecisionLoc, float radius);
	void SetDecisionType(EAttackDecisionType type) { DecisionType = type;  };
};
