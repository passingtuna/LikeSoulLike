// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterDefaultBase.h"
#include "DA_DefaultMotion.h"
#include "DA_ActionData.h"
#include "AnimInstanceDefaultBase.h"
#include "Manager_UI.h"
#include "WeaponDefaultBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/WidgetComponent.h"
#include "UW_LockONHpBar.h"
#include "Manager_Calculate.h"
#include "Components/CapsuleComponent.h"
// Sets default values
ACharacterDefaultBase::ACharacterDefaultBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACharacterDefaultBase::BeginPlay()
{
	Super::BeginPlay();

	IsRunning = false;
	IsGuarding = false;
	IsAvoiding = false;
	IsParrying = false;
	IsBouncing = false;
	IsAttacking = false;
	IsMoveable = true;
	IsAvoidable = true;
	IsAttackable = true;
	CanRestoreStemina = true;

	CurrentState.Toughness = MaxState.Toughness;

	AnimInstance = Cast< UAnimInstanceDefaultBase>(GetMesh()->GetAnimInstance());

	CharacterMovement = GetCharacterMovement();

	UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UManager_UI>();
	CalculatManager = GetWorld()->GetGameInstance()->GetSubsystem<UManager_Calculate>();

	
	LastInputSecond = 0;
	AddWeaponToMainSlot(BaseWeapon, 0);
	AnimInstance->SetOwnerCharacter(this);
	
	LockOnWidget_HpBar = Cast<UWidgetComponent>(GetDefaultSubobjectByName(TEXT("LockOnWidget_HP")));
	if(LockOnWidget_HpBar)
	{
		LockOnWidget_HpBar->SetVisibility(false);
	}

	LockOnWidget_Dot   = Cast<UWidgetComponent>(GetDefaultSubobjectByName(TEXT("LockOnWidget_Dot")));
	if (LockOnWidget_Dot)
	{
		LockOnWidget_Dot->SetVisibility(false);
	}
	AnimMontagePlaySpeed = 1;
}

// Called every frame
void ACharacterDefaultBase::Tick(float DeltaTime)
{
	if (IsDead) return;
	Super::Tick(DeltaTime);
	if (CanRestoreStemina && CurrentState.Stemina < MaxState.Stemina) ModifyCurrentStemina(2);

}

// Called to bind functionality to input
void ACharacterDefaultBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ACharacterDefaultBase::AddWeaponToMainSlot(TSubclassOf<AWeaponDefaultBase> weapon, int slotNum)
{
	if (!weapon)return;
	AWeaponDefaultBase* TempWeapon = GetWorld()->SpawnActor<AWeaponDefaultBase>(weapon, FTransform());

	if (TempWeapon)
	{
		CurrentWeapon = TempWeapon;
		CurrentWeapon->SettingWeaponLocation(this);
		if (TempWeapon->IdleMotion) AnimInstance->WeaponIdleMotion = TempWeapon->IdleMotion;
		if (TempWeapon->WalkMotion)	AnimInstance->WeaponWalkMotion = TempWeapon->WalkMotion;
		if (TempWeapon->RunMotion)	AnimInstance->WeaponRunMotion =  TempWeapon->RunMotion;

	}
}

void ACharacterDefaultBase::CalculateResist()
{
}


FName ACharacterDefaultBase::GetDirectionName( FVector Target)
{
	float resultDotFront = FVector::DotProduct(GetActorForwardVector(), Target);
	float resultDotRight = FVector::DotProduct(GetActorRightVector(), Target);
	if (FMath::Abs(resultDotFront) >= FMath::Abs(resultDotRight))
	{
		return resultDotFront >= 0 ? "Default" : "Back";
	}
	else
	{
		return resultDotRight >= 0 ? "Right" : "Left";
	}
}

void ACharacterDefaultBase::ReceiveAttack(const FDamageData damageData, const FVector hitDirection)
{
	if (IsDead) return;
	float resultDotFront = FVector::DotProduct(GetActorForwardVector(), hitDirection);
	float resultDotRight = FVector::DotProduct(GetActorRightVector(), hitDirection);

	if (IsGuarding && damageData.IsGaurdable)
	{
		if (resultDotFront >= 0.5f) //전방 60도에서 공격이 들어왔다면
		{
			if(CurrentPlayActionData)
			{
				ModifyCurrentStemina(20*-1);
				if (CurrentState.Stemina <= 0) PlayAnimMontage(DefaultMotion->StunMotion, 1); //가드중 스태미너가 0이되면 스턴모션 재생
				return;
			}
		}
	}
	else if (IsAvoiding && damageData.IsAvoidable)
	{
		return;
	}
	else if (IsParrying && damageData.IsGaurdable)
	{
		return;
	}
	int finalDamage = CalculateFinalDamage(damageData);
	ModifyCurrentHealth(-finalDamage);
	ModifyCurrentToughness(-damageData.StaggerDamage);
	if (CurrentState.Toughness <= 0)
	{
		PlayAnimMontage(DefaultMotion->HitMotion, 1, GetDirectionName(hitDirection.GetSafeNormal()));
		SetCurrentToughness(MaxState.Toughness);
	}
}


int ACharacterDefaultBase::CalculateFinalDamage(const FDamageData& damageData)
{
	int TempDamage = 0;
	TempDamage += (damageData.PhysicalDamage *	1000 - CurrentResist.PhysicalResist)	/ 1000;
	TempDamage += (damageData.FireDamage *		1000 - CurrentResist.FireResist)		/ 1000;
	TempDamage += (damageData.LightningDamage * 1000 - CurrentResist.LightningResist)	/ 1000;
	TempDamage += (damageData.MagicDamage *		1000 - CurrentResist.MagicResist)		/ 1000;
	TempDamage += (damageData.DevineDamage *	1000 - CurrentResist.DevineResist)		/ 1000;

	return TempDamage;
}

void ACharacterDefaultBase::ModifyCurrentHealth(int param)
{;
	CurrentState.HP += param;
	if (CurrentState.HP <= 0)
	{
		CurrentState.HP = 0;
		DiedProcess();
	}
	else if (CurrentState.HP >= MaxState.HP)
	{
		CurrentState.HP = MaxState.HP;
	}

	if (GetWorld()->GetFirstPlayerController()->GetPawn() == this)
	{
		UIManager->SetPlayerHP(CurrentState.HP, MaxState.HP);
	}
	else
	{
		UpdateLockOnUI();
	}
}
void ACharacterDefaultBase::ModifyCurrentStemina(int stemina)
{
	if (stemina == 0) return; //소비가 0일경우 스테미나 회복에 영향없게
	CurrentState.Stemina += stemina;

	UE_LOG(LogTemp, Warning, TEXT("ModifyCurrentStemina : %d"), CurrentState.Stemina);
	if (CurrentState.Stemina <= 0)
	{
		CurrentState.Stemina = 0; 
		CanRestoreStemina = false;
	}
	else if (CurrentState.Stemina >= MaxState.Stemina)
	{
		CurrentState.Stemina = MaxState.Stemina;
	}
	GetWorld()->GetTimerManager().SetTimer(SteminaTimer, [this]()
		{
			CanRestoreStemina = true;
		}, 2, false);
}
void ACharacterDefaultBase::ModifyCurrentMana(int mana)
{
	CurrentState.Mana += mana;
	if (CurrentState.Mana <= 0)
	{
		CurrentState.Mana = 0;
	}
	else if (CurrentState.Mana >= MaxState.Mana)
	{
		CurrentState.Mana = MaxState.Mana;
	}
}
void ACharacterDefaultBase::ModifyCurrentToughness(int toughness)
{
	CurrentState.Toughness += toughness;
	if (CurrentState.Toughness <= 0)
	{
		CurrentState.Toughness = 0;
	}
	else if (CurrentState.Toughness >= MaxState.Toughness)
	{
		CurrentState.Toughness = toughness;
	}
}
void ACharacterDefaultBase::SetCurrentToughness(int toughness)
{
	CurrentState.Toughness = toughness;
}
void ACharacterDefaultBase::DiedProcess()
{
	IsDead = true;
	AnimInstance->StopAllMontages(0);
	PlayAnimMontage(DefaultMotion->DiedMotion, 1);
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this]()
		{
			GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
			GetMesh()->SetSimulatePhysics(true);
			GetMesh()->bBlendPhysics = true;
		},
	2.0f,
	false);
}

void ACharacterDefaultBase::SetCrouchState(bool param)
{
	IsCrouch = param;
	AnimInstance->SetIsCrouch(IsCrouch);
};

void ACharacterDefaultBase::ReadyToAction()
{
	IsMoveable   = false;
	IsRunable = false;
	IsAvoidable	 = false;
	IsAttackable = false;
	SetCrouchState(false);
	CanRestoreStemina = false;
	IsGuarding = false;
	IsAvoiding = false;
	IsParrying = false;
	IsBouncing = false;
	IsAttacking = false;
	CharacterMovement->bOrientRotationToMovement = false;
}

void ACharacterDefaultBase::ActionEnd()
{
	IsMoveable = true;
	IsRunable = true;
	IsAvoidable = true;
	IsAttackable = true;
	IsGuarding = false;
	IsAvoiding = false;
	IsParrying = false;
	IsBouncing = false;
	IsAttacking = false;
	CharacterMovement->bOrientRotationToMovement = true;
	CurrentPlayActionData = nullptr;
	CurrentWeapon->ResetWeaponEvent();
}

void ACharacterDefaultBase::ActionConsumeProcess()
{
	int consumeStemina = 0;
	int consumeMana = 0;
	if (CurrentPlayActionData)
	{
		consumeStemina	= CurrentPlayActionData->ConsumeStemina;
		consumeMana		= CurrentPlayActionData->ConsumeMana;
	}
	if(consumeStemina > 0)
		ModifyCurrentStemina(consumeStemina * -1);
	if (consumeMana > 0)
		ModifyCurrentMana(consumeMana * -1);
}
void ACharacterDefaultBase::AvoidMovement(FVector Vector, float Speed)
{
	if (IsDead) return;
	AddMovementInput(Vector, Speed);
	CharacterMovement->BrakingDecelerationWalking = 0.f; // 바로 멈추지 않도록
}
bool ACharacterDefaultBase::ExcuteActionMotionByType(UDA_ActionData* actionData, ETriggerEvent Trigger, const FInputActionInstance& instance)
{
	if (IsDead) return false;

	if (Trigger == ETriggerEvent::Started)
	{
		SetCurrentToughness(MaxState.Toughness);
	}

	switch (actionData->ActionType)
	{
	case EActionType::AT_Instance:
		{
			switch (Trigger)
			{
			case ETriggerEvent::Started:
			{
				ReadyToAction();
				CurrentPlayActionData = actionData;
				PlayAnimMontage(actionData->AnimMontage, AnimMontagePlaySpeed, "Default");
				SetCrouchState(false);
			}
			break;
			default: break;
			}
		}
		break;
	case EActionType::AT_Release:
		{
			switch (Trigger)
			{
			case ETriggerEvent::Completed:
			{
				ReadyToAction();
				CurrentPlayActionData = actionData;
				PlayAnimMontage(actionData->AnimMontage, AnimMontagePlaySpeed, "Default");
				SetCrouchState(false);
			}
			break;
			default: break;
			}
		}
		break;
	case EActionType::AT_Avoid:
		{
			switch (Trigger)
			{
			case ETriggerEvent::Completed:
			{
				GetAnimInstance()->StopAllMontages(0);
				ReadyToAction();
				CurrentPlayActionData = actionData;
				PlayAnimMontage(actionData->AnimMontage, 1, GetDirectionName(LastInputDir));
				SetCrouchState(false);
			}
			break;
			default: break;
			}
		}
		break;
	case EActionType::AT_Charge:
		{
			switch (Trigger)
			{
				case ETriggerEvent::Started:
				{
					CurrentPlayActionData = actionData;
					PlayAnimMontage(actionData->AnimMontage, AnimMontagePlaySpeed, "Default");
					ReadyToAction();
				}
				break;
				case ETriggerEvent::Triggered:
				{
				}
				break;
				case ETriggerEvent::Completed:
				{
					FName CurrentSection = AnimInstance->Montage_GetCurrentSection(actionData->AnimMontage);
					if(CurrentSection == "Default")
					{
						StopAnimMontage(actionData->AnimMontage);
						ActionEnd();
					}
					else
					{
						if (AnimInstance && AnimInstance->Montage_IsPlaying(actionData->AnimMontage) && CurrentSection == "Loop")
						{
							CurrentPlayActionData = actionData;
							AnimInstance->Montage_JumpToSection("Completed");
						}
						return true;
					}
				}
				break;
			default: break;
			}
		}
		break;
	case EActionType::AT_Hold:
		{
			switch (Trigger)
			{
				case ETriggerEvent::Started:
				{
					CurrentPlayActionData = actionData;
					PlayAnimMontage(actionData->AnimMontage, AnimMontagePlaySpeed, "Default");
					return true;
				}
				break;
				case ETriggerEvent::Triggered:
				{
					if ((actionData->ConsumeStemina > 0 && CurrentState.Stemina <= actionData->ConsumeStemina) ||
						(actionData->ConsumeMana > 0 && CurrentState.Mana <= actionData->ConsumeMana))
					{
						ReadyToAction();
						CurrentPlayActionData = actionData;
						PlayAnimMontage(actionData->AnimMontage, AnimMontagePlaySpeed, "Completed");
						return true;
					}
				}
				break;
				case ETriggerEvent::Completed:
				{
					if (AnimInstance && AnimInstance->Montage_IsPlaying(actionData->AnimMontage))
					{
						ReadyToAction();
						CurrentPlayActionData = actionData;
						AnimInstance->Montage_JumpToSection("Completed");
					}
					return true;
				}
				break;
				default: break;
			}
		}
		break;
	}
	return false;
}



void ACharacterDefaultBase::UpdateLockOnUI()
{
	if (LockOnWidget_HpBar)
	{
		if (UUserWidget* UserWidget = LockOnWidget_HpBar->GetUserWidgetObject())
		{
			if (UUW_LockONHpBar* EnemyWidget = Cast<UUW_LockONHpBar>(UserWidget))
			{
				float Percent = ((float)CurrentState.HP / (float)MaxState.HP);
				EnemyWidget->HP_GaugeWidget->SetPercent(Percent);
			}
		}
	}
}


void ACharacterDefaultBase::StartLockOnUI()
{
	if (LockOnWidget_HpBar)
	{
		LockOnWidget_HpBar->SetVisibility(true);
	}
	if (LockOnWidget_HpBar)
	{
		LockOnWidget_Dot->SetVisibility(true);
	}
	UpdateLockOnUI();

}

void ACharacterDefaultBase::EndLockOnUI()
{
	if (LockOnWidget_HpBar)
	{
		LockOnWidget_HpBar->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		LockOnWidget_HpBar->SetVisibility(false);
	}
	if (LockOnWidget_Dot)
	{
		LockOnWidget_Dot->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		LockOnWidget_Dot->SetVisibility(false);
	}
}

void ACharacterDefaultBase::SetIsAttacking(bool param) 
{
	if (IsDead) return;
	IsAttacking = param; 
	FDamageData  CalculateDataInfo = CalculatManager->CaculateAttackDamage(this);
	FDamageData* CalculateDataInfoPtr = &CalculateDataInfo;
	if (CurrentWeapon)
	{
		CurrentWeapon->SetIsAttacking(IsAttacking, CalculateDataInfoPtr , this);
	}
}

