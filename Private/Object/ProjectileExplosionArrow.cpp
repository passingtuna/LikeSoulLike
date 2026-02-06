// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileExplosionArrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CharacterDefaultBase.h"
#include "Manager_Calculate.h"
#include "AttackDecisionComponent.h"
#include "Components/AudioComponent.h"

void AProjectileExplosionArrow::BeginPlay()
{
	Super::BeginPlay();
	Explosion.FireDamage = 500;
}
void AProjectileExplosionArrow::OnHitAcotor(const FHitResult& temp)
{
	ACharacterDefaultBase* Char = Cast<ACharacterDefaultBase>(temp.GetActor());

	if (HitAudioComp)
	{
		HitAudioComp->Play();
	}
	FVector ImpactLocation = temp.ImpactPoint;
	SetActorLocation(ImpactLocation);
	ProjectileMoveComp->Deactivate();
	AttackDecisionComp->EndAttack(); //화살 공격 종료
	AttackDecisionComp->DataInfo = Explosion;
	AttackDecisionComp->PerformAreaOnceAtLocation (ImpactLocation, 300.0f);
	ResetProjectile();
}