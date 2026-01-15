// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterDefaultBase.h"
#include "GenericTeamAgentInterface.h"
#include "CharacterNonPlayableBase.generated.h"

class ABase_AIController;
class UManager_Enemy;
DECLARE_DELEGATE(FOnNpcEventDeligate);
UCLASS()
class ACharacterNonPlayableBase : public ACharacterDefaultBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterNonPlayableBase();

	virtual FGenericTeamId GetGenericTeamId() const override;

	FTransform SpawnLocation;
	UManager_Enemy* EnemyManager;

	FOnNpcEventDeligate OnDeadDeligate;
	FOnNpcEventDeligate OnRespawnDeligate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	bool IsBoss = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	int32 OwnSoul;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ABase_AIController* BaseController;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void ModifyCurrentHealth(int32 param) override;
	virtual void ExcuteWeakAttack();
	virtual void ReceiveAttack(ACharacterDefaultBase* Attacker, const FDamageData damage, const FVector hitDirection)override;

	virtual void DiedProcess() override;

	bool GetIsAvoidable() {return IsAvoidable;};
	bool GetIsMoveable() { return IsMoveable; };
	bool GetIsAttackable(){ return IsAttackable; };
	bool GetIsRunable(){ return IsRunable; };


	void TryLockOn(ACharacterDefaultBase* lockOnTarget);
	void ReleaseLockOn();
	void UpdateLockOnRotation(float DeltaTime);

	void Respawn();
	void BossEngage();
};
