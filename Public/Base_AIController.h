// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "LikeSoulLikeType.h"
#include "Base_AIController.generated.h"

/**
 * 
 */
class ACharacterNonPlayableBase;
class ACharacterPlayableBase;
class UNavigationSystemV1;
class UBlackboardComponent;
class UAIPerceptionComponent;
struct FAIStimulus;

enum LastMoveToOrder
{LMO_Spawn,LMO_Target,LMO_LastSeenLoc };
UCLASS()
class LIKESOULLIKE_API ABase_AIController : public AAIController
{
	GENERATED_BODY()
public:
	ABase_AIController();
	virtual FGenericTeamId GetGenericTeamId() const override
	{
		return FGenericTeamId(static_cast<uint8>(ControllerTeam));
	}
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UNavigationSystemV1* NavSys;
	TObjectPtr<ACharacterNonPlayableBase> NpcActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAIPerceptionComponent* AIPerceptionComp;
	ACharacterPlayableBase* TargetChar;
	FVector SpawnLocation;
	FRotator SpawnRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETeam ControllerTeam = ETeam::T_Monster;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBlackboardData* BlackboardAsset;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBlackboardComponent* BlackboardComp;

	bool ExcuteAttack();
	bool ExcuteMoveTo(UObject* object);
	bool ExcuteMoveTo(FVector vector);
	void SetTarget(AActor* Target);

	LastMoveToOrder LastMoveOrdered;

	bool ReturnToSpawn();
	UFUNCTION()
	void OnReturnCompletedCallback(FAIRequestID RequestID, const EPathFollowingResult::Type Result);

	UFUNCTION()
	void UpdateTarget(AActor* Actor, FAIStimulus Stimulus);
};
