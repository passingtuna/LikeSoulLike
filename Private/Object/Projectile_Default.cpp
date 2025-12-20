// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_Default.h"	
#include "GameFramework/ProjectileMovementComponent.h"
#include "CharacterDefaultBase.h"
#include "Manager_Calculate.h"
#include "AttackDecisionComponent.h"


// Sets default values
AProjectile_Default::AProjectile_Default()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	IsReadyToFire = true;

	ProjectileMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	AttackDecisionComp = CreateDefaultSubobject<UAttackDecisionComponent>(TEXT("AttackDeicisionComp"));
}

// Called when the game starts or when spawned
void AProjectile_Default::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMoveComp = GetComponentByClass<UProjectileMovementComponent>();
	AttackDecisionComp = GetComponentByClass<UAttackDecisionComponent>();
	if (AttackDecisionComp)
	{
		AttackDecisionComp->OnHitSweep.BindLambda([this](const FHitResult& temp) {OnHitAcotor(temp); }); //히트 콜백 연결
	}
}

// Called every frame
void AProjectile_Default::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!ProjectileMoveComp || !ProjectileMoveComp->IsActive())
		return;

	float TraveledDist = FVector::Dist(LaunchedLoc, GetActorLocation());

	if (IsReadyToFire == false && TraveledDist >= MaxRange)
	{
		ResetProjectile(); // or Destroy()
	}
}


void AProjectile_Default::Fire(ACharacterDefaultBase* owner, FDamageData damageData, const FVector& loc, const FVector& Dir, float Speed)
{
	SetActorLocation(loc);
	LaunchedLoc = loc;
	ProjectileMoveComp->Velocity = Dir * Speed;
	DamageData = damageData;
	SetOwner(owner);
	OwnerCharacter = owner;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	IsReadyToFire = false;
	ProjectileMoveComp->Activate();
	AttackDecisionComp->StartAttack(damageData, owner);
}

void AProjectile_Default::ResetProjectile()
{
	UE_LOG(LogTemp,Warning,TEXT("리셋 프로젝타일"));
	IsReadyToFire = true;
	ProjectileMoveComp->StopMovementImmediately();
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	AttackDecisionComp->EndAttack();
}

void AProjectile_Default::ReadyProjectile()
{
	IsReadyToFire = false;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	UE_LOG(LogTemp, Warning, TEXT("레디 프로젝타일"));

}
void AProjectile_Default::OnHitAcotor(const FHitResult& temp)
{
	ACharacterDefaultBase * Char = Cast<ACharacterDefaultBase>(temp.GetActor());

	FVector ImpactLocation = temp.ImpactPoint;
	SetActorLocation(ImpactLocation);
	ProjectileMoveComp->Deactivate();

	if (Char)//캐릭터와 충돌이면 본에다 붙여줌
	{
		if (USkeletalMeshComponent* MeshComp = Cast<USkeletalMeshComponent>(temp.GetComponent()))
		{
			FName BoneName = temp.BoneName;
			if (BoneName != NAME_None)
			{
				AttachToComponent(
					MeshComp,
					FAttachmentTransformRules::KeepWorldTransform,
					BoneName
				);
			}
		}
	}
	else
	{
		AttachToActor(temp.GetActor(), FAttachmentTransformRules::KeepWorldTransform); //다른오브젝트라면 거기에 박힘
	}
	AttackDecisionComp->EndAttack();
	FTimerHandle TempHandle;
	GetWorld()->GetTimerManager().SetTimer(TempHandle, [this]() { ResetProjectile(); }, 3.0f, false);
}