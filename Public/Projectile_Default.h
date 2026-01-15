// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"	
#include "LikeSoulLikeType.h"
#include "Projectile_Default.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UManager_Calculate;
class UAttackDecisionComponent;
class UAudioComponent;
UCLASS()
class LIKESOULLIKE_API AProjectile_Default : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile_Default();

private:
	bool IsReadyToFire;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* HitAudioComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAttackDecisionComponent* AttackDecisionComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProjectileMovementComponent* ProjectileMoveComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxRange;
	FVector LaunchedLoc;

	USphereComponent* Collision;
	FDamageData DamageData;
	ACharacterDefaultBase* OwnerCharacter;

	void Fire(ACharacterDefaultBase* owner, FDamageData damageData ,const FVector& loc , const FVector & Dir, float Speed);
	void ReadyProjectile();
	void ResetProjectile();
	bool GetIsReadyToFire() {return IsReadyToFire;};

	UFUNCTION()
	virtual void OnHitAcotor(const FHitResult& temp);


};
