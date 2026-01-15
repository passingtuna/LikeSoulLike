// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterNonPlayableBase.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Base_AIController.h"
#include "CharacterPlayableBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Manager_Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Manager_UI.h"
// Sets default values
ACharacterNonPlayableBase::ACharacterNonPlayableBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

FGenericTeamId ACharacterNonPlayableBase::GetGenericTeamId() const
{
	if (BaseController)
	{
		return FGenericTeamId(BaseController->GetGenericTeamId());
	}
	return FGenericTeamId(static_cast<uint8>(ETeam::T_Neutral));
}

// Called when the game starts or when spawned
void ACharacterNonPlayableBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentState.HP = MaxState.HP;
	CurrentState.Mana = MaxState.Mana;
	CurrentState.Stemina = MaxState.Stemina;
	CurrentState.Toughness = MaxState.Toughness;
	MaxState.Toughness = 1;
	MaxState.Weight = 100;
	AddWeaponToMainSlot(BaseWeapon, 0);
	BaseController = Cast<ABase_AIController>(Controller);

	SpawnLocation = GetActorTransform();
	EnemyManager = GetGameInstance()->GetSubsystem<UManager_Enemy>();
	EnemyManager->AddEnemy(this);
}

// Called every frame
void ACharacterNonPlayableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//ExcuteWeakAttack();
	if (IsLockOn)
	{
		if (LockOnTargetChar)
		{
			UpdateLockOnRotation(DeltaTime);
		}
		else
		{
			ReleaseLockOn();
		}
	}
	else
	{
		bUseControllerRotationYaw = true;
		FVector DirToTarget = GetActorForwardVector();
		FRotator DesiredRot = DirToTarget.Rotation();
		if(BaseController)BaseController->SetControlRotation(DesiredRot);
	}
}

// Called to bind functionality to input
void ACharacterNonPlayableBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void ACharacterNonPlayableBase::ModifyCurrentHealth(int32 param)
{
	Super::ModifyCurrentHealth(param);
	UpdateLockOnUI();

}
void ACharacterNonPlayableBase::ExcuteWeakAttack()
{
	CharacterMovement->StopMovementImmediately();

	if (LockOnTargetChar)
	{
		FVector Location = GetActorLocation();
		FVector TargetLoc = LockOnTargetChar->GetActorLocation();
		FRotator DesiredRot = UKismetMathLibrary::FindLookAtRotation(Location, TargetLoc);
		GetController()->SetControlRotation(DesiredRot);
		SetActorRotation(DesiredRot);
	}

	ExcutingWeaponBaseAction(EActionInputType::AIT_WeakAttack, ETriggerEvent::Started);
}

void ACharacterNonPlayableBase::ReceiveAttack(ACharacterDefaultBase* Attacker, const FDamageData damageData, const FVector hitDirection)
{
	Super::ReceiveAttack(Attacker, damageData, hitDirection);
	if(BaseController)BaseController->SetTarget(Attacker);
	if(IsBoss)	GetWorld()->GetGameInstance()->GetSubsystem<UManager_UI>()->SetBossHP(CurrentState.HP, MaxState.HP);
}

void ACharacterNonPlayableBase::TryLockOn(ACharacterDefaultBase* lockOnTarget)
{
	if (lockOnTarget)
	{
		IsLockOn = true;
		LockOnTargetChar = lockOnTarget;
	}
}

void ACharacterNonPlayableBase::ReleaseLockOn()
{
	IsLockOn = false;
	LockOnTargetChar = nullptr;
}

void ACharacterNonPlayableBase::UpdateLockOnRotation(float DeltaTime)
{
	FVector Location = GetActorLocation();
	FVector TargetLoc = LockOnTargetChar->GetActorLocation();
	FRotator DesiredRot = UKismetMathLibrary::FindLookAtRotation(Location, TargetLoc);
	GetController()->SetControlRotation(DesiredRot);
}

void ACharacterNonPlayableBase::Respawn()
{
	if (IsBoss && IsDead)
	{
		Destroy();
		return;
	}
	if (OnRespawnDeligate.IsBound()) OnRespawnDeligate.Execute();
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->bBlendPhysics = false;
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	//GetCapsuleComponent()->SetWorldLocation(SpawnLocation);
	//GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()), FRotator(0.f, -90.f, 0.f));
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	SetActorHiddenInGame(false);
	SetActorTransform(SpawnLocation);
	ResetStates();
}

void ACharacterNonPlayableBase::DiedProcess()
{
	Super::DiedProcess();

	if (OnDeadDeligate.IsBound()) OnDeadDeligate.Execute();

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this]()
		{
			GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
			GetMesh()->SetSimulatePhysics(true);
			GetMesh()->bBlendPhysics = true;
			EnemyManager->GiveEnemySoul(OwnSoul);
		},
		2.0f,
		false);
}



void ACharacterNonPlayableBase::BossEngage()
{
	GetWorld()->GetGameInstance()->GetSubsystem<UManager_UI>()->ShowBossHPUI(true);
	GetWorld()->GetGameInstance()->GetSubsystem<UManager_UI>()->SetBossName(Name);
	GetWorld()->GetGameInstance()->GetSubsystem<UManager_UI>()->SetBossHP(CurrentState.HP, MaxState.HP);
}