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
#include "Components/AudioComponent.h"
#include "GrabProcessComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterNonPlayableBase.h"
#include "Engine/OverlapResult.h"
#include "CharacterPlayableBase.h"
#include "DefaultSoundData.h"

// Sets default values
ACharacterDefaultBase::ACharacterDefaultBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GrapComp = CreateDefaultSubobject<UGrabProcessComponent>(TEXT("GrapComponent"));
	GrapComp->SetupAttachment(GetRootComponent());


	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComp->SetupAttachment(GetRootComponent());

	AudioEffectComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioEffect"));
	AudioEffectComp->SetupAttachment(GetRootComponent());
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
	IsRunable = true;
	CanRestoreStemina = true;

	CurrentState.Toughness = MaxState.Toughness;

	AnimInstance = Cast< UAnimInstanceDefaultBase>(GetMesh()->GetAnimInstance());

	CharacterMovement = GetCharacterMovement();
;
	CalculatManager = GetWorld()->GetGameInstance()->GetSubsystem<UManager_Calculate>();

	LastInputSecond = 0;
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
	if (ChargeData.Started) ChargeData.ElaseTime += DeltaTime;
}

// Called to bind functionality to input
void ACharacterDefaultBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ACharacterDefaultBase::ExcuteItemAffect(FItemAffectData& data)
{
	switch (data.AffectType)
	{
	case EAffectionType::AT_CurrentHp:
	{
		PlayAnimMontage(DefaultMotion->UsePotion);
		GetWorld()->GetTimerManager().SetTimer(ItemUseTimer, [this, data]() {ModifyCurrentHealth(data.Value); }, 1, false);
	}
	break;
	case EAffectionType::AT_CurrentMana:
	{
		PlayAnimMontage(DefaultMotion->UsePotion);
		GetWorld()->GetTimerManager().SetTimer(ItemUseTimer, [this, data]() {ModifyCurrentMana(data.Value); }, 1, false);
	}
	break;
	case EAffectionType::AT_FireDamage:
		if (!CurrentWeapon->bIsBuffAble) return false;
		CurrentWeapon->OnWeaponBuff(data);
		PlayAnimMontage(DefaultMotion->UsePaper);
		break;
	case EAffectionType::AT_LightningDamage:
		if (!CurrentWeapon->bIsBuffAble) return false;
		CurrentWeapon->OnWeaponBuff(data);
		PlayAnimMontage(DefaultMotion->UsePaper);
		break;
	case EAffectionType::AT_MagicDamage:
		if (!CurrentWeapon->bIsBuffAble) return false;
		CurrentWeapon->OnWeaponBuff(data);
		PlayAnimMontage(DefaultMotion->UsePaper);
		break;
	case EAffectionType::AT_DevineDamage:
		if (!CurrentWeapon->bIsBuffAble) return false;
		CurrentWeapon->OnWeaponBuff(data);
		PlayAnimMontage(DefaultMotion->UsePaper);
		break;
	}
	return true;
}

void ACharacterDefaultBase::AddWeaponToMainSlot(TSoftClassPtr<AWeaponDefaultBase> weapon, int32 slotNum)
{
	TSubclassOf<AWeaponDefaultBase> SpwanBp = weapon.Get(); // 로드 확인
	if (!SpwanBp) SpwanBp = weapon.LoadSynchronous(); //동기 로드 시도
	if (!SpwanBp) return; //동기 로드도 실패면 리턴

	AWeaponDefaultBase* TempWeapon = GetWorld()->SpawnActor<AWeaponDefaultBase>(SpwanBp, FTransform());

	if (TempWeapon)
	{
		CurrentWeapon = TempWeapon;
		CurrentWeapon->SettingWeaponData(this);

		if (TempWeapon->IdleMotion) AnimInstance->WeaponIdleMotion = TempWeapon->IdleMotion;
		if (TempWeapon->WalkMotion)	AnimInstance->WeaponWalkMotion = TempWeapon->WalkMotion;
		if (TempWeapon->RunMotion)	AnimInstance->WeaponRunMotion =  TempWeapon->RunMotion;

	}
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

void ACharacterDefaultBase::ReceiveAttack(ACharacterDefaultBase* Attacker, const FDamageData damageData, const FVector hitDirection)
{
	if (IsDead) return;
	if (IsGuarding && damageData.IsGuardable)
	{
		float resultDotFront = FVector::DotProduct(GetActorForwardVector(), hitDirection);
		if (resultDotFront <= 0.5f) //전방 60도에서 공격이 들어왔다면
		{
			ModifyCurrentStemina(20 * -1);
			if (CurrentState.Stemina <= 0) SetStun(); //가드중 스태미너가 0이되면 스턴
			return;
		}
	}
	else if (IsAvoiding && damageData.IsAvoidable)
	{
		return;
	}
	else if (IsParrying && damageData.IsParryable)
	{
		Attacker->SetStun();
		return;
	}
	else if (IsBouncing)
	{
		//Attacker->PlayAnimMontage(DefaultMotion->HitMotion, 1, GetDirectionName(hitDirection.GetSafeNormal()));
		IsAttackable = true;
		ModifyCurrentStemina(20);
		FString sound = "Bound_" + FString::FromInt(FMath::RandRange(1, 2));
		UE_LOG(LogTemp, Warning, TEXT("Damage :  %s"), *sound);
		PlaySound(FName(sound));
		return;
	}
	ModifyCurrentToughness(-damageData.StaggerDamage);
	if (CurrentState.Toughness <= 0)
	{
		PlayAnimMontage(DefaultMotion->HitMotion, 1, GetDirectionName(hitDirection.GetSafeNormal()));
		SetCurrentToughness(MaxState.Toughness);
		GetWorld()->GetTimerManager().ClearTimer(ItemUseTimer);
	}
	int32 finalDamage = CalculateFinalDamage(damageData);
	UE_LOG(LogTemp, Warning, TEXT("Damage :  %d"), finalDamage);
	ModifyCurrentHealth(-finalDamage);
}


int32 ACharacterDefaultBase::CalculateFinalDamage(const FDamageData& damageData)
{
	int32 TempDamage = 0;
	TempDamage += (damageData.PhysicalDamage *	1000 - CurrentResist.PhysicalResist)	/ 1000;
	TempDamage += (damageData.FireDamage *		1000 - CurrentResist.FireResist)		/ 1000;
	TempDamage += (damageData.LightningDamage * 1000 - CurrentResist.LightningResist)	/ 1000;
	TempDamage += (damageData.MagicDamage *		1000 - CurrentResist.MagicResist)		/ 1000;
	TempDamage += (damageData.DevineDamage *	1000 - CurrentResist.DevineResist)		/ 1000;

	return TempDamage;
}

void ACharacterDefaultBase::ModifyCurrentHealth(int32 param)
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
}
void ACharacterDefaultBase::ModifyCurrentStemina(int32 stemina)
{
	if (stemina == 0 ) return;//소모값이 없으면 리턴
	if (stemina < 0) CanRestoreStemina = false; // 어떤이유던 소모가 됐으면 회복 멈춤

	CurrentState.Stemina += stemina;

	if (CurrentState.Stemina <= 0)
	{
		CurrentState.Stemina = 0; 
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
void ACharacterDefaultBase::ModifyCurrentMana(int32 mana)
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
void ACharacterDefaultBase::ModifyCurrentToughness(int32 toughness)
{
	CurrentState.Toughness += toughness*2;
	if (CurrentState.Toughness <= 0)
	{
		CurrentState.Toughness = 0;
	}
	else if (CurrentState.Toughness >= MaxState.Toughness)
	{
		CurrentState.Toughness = MaxState.Toughness;
	}
}
void ACharacterDefaultBase::SetCurrentToughness(int32 toughness)
{
	CurrentState.Toughness = toughness;
}
void ACharacterDefaultBase::DiedProcess()
{
	IsDead = true;
	AnimInstance->StopAllMontages(0);
	PlayAnimMontage(DefaultMotion->DiedMotion, 1);

	SetIsGrabable(false);
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
	SetIsAttacking(false); 
	SetIsGrabable(false);
	CharacterMovement->bOrientRotationToMovement = true;
	CurrentPlayActionData = nullptr;
	CurrentWeapon->ResetWeaponEvent();
}

void ACharacterDefaultBase::ActionConsumeProcess()
{
	int32 consumeStemina = 0;
	int32 consumeMana = 0;
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
	if (LockOnWidget_Dot)
	{
		LockOnWidget_Dot->SetVisibility(true);
	}
	UpdateLockOnUI();

}

void ACharacterDefaultBase::EndLockOnUI()
{
	if (LockOnWidget_HpBar)
	{
		LockOnWidget_HpBar->SetVisibility(false);
	}
	if (LockOnWidget_Dot)
	{
		LockOnWidget_Dot->SetVisibility(false);
	}
}

void ACharacterDefaultBase::SetIsAttacking(bool param) 
{
	if (IsDead) return;
	if (!CalculatManager) return;
	IsAttacking = param;
	if (CurrentWeapon)
	{
		CurrentWeapon->DecisionAttack(IsAttacking, this, CurrentPlayActionData);
	}
}
bool ACharacterDefaultBase::CheckConsumeableResource(int32 consumeMana)
{
	if (CurrentState.Stemina <= 0) return false;
	if (CurrentState.Mana < consumeMana) return false;
	return true;
}

FName ACharacterDefaultBase::GetCurrentSection(UAnimMontage* montage)
{
	if (!AnimInstance) return "";
	if(!AnimInstance->Montage_IsPlaying(montage)) return "";
	return AnimInstance->Montage_GetCurrentSection(montage);

}
void ACharacterDefaultBase::ExcutingWeaponBaseAction(EActionInputType actionInputType, ETriggerEvent Trigger)
{
	if (IsDead) return;
	UDA_ActionData* playMotionData = nullptr;
	if (Trigger == ETriggerEvent::Started && CurrentPlayActionData) // 재생중인 액션 데이터가 존재할때 파생될수있는 액션이있다면 새로 입력받았을때만 체크
	{
		for (auto& Elem : CurrentPlayActionData->NextAction)
		{
			if (Elem.InputType == actionInputType) //파생 키입력 체크후 실행
			{
				playMotionData = Elem.ComboAction;
			}
		}
	}
	if (!playMotionData) //파생 모션이 없으면 
	{
		if (CurrentWeapon) // 현재 장착 무기 기준으로 액션을 가져온다
		{
			switch (actionInputType)
			{
			case EActionInputType::AIT_WeakAttack:
				playMotionData = CurrentWeapon->WeakAttackMotion;
				break;
			case EActionInputType::AIT_StrongAttack:
				playMotionData = CurrentWeapon->StrongAttackMotion;
				break;
			case EActionInputType::AIT_WeaponAction:
				playMotionData = CurrentWeapon->WeaponActionMotion;
				break;
			case EActionInputType::AIT_WeaponSkill:
				playMotionData = CurrentWeapon->WeaponSkillMotion;
				break;
			case EActionInputType::AIT_Avoid:
				playMotionData = CurrentWeapon->WeaponAvoidMotion;
				break;
			default: break;
			}
		}
	}

	if (!playMotionData) return;

	if (Trigger == ETriggerEvent::Started)
	{

		if (!IsAttackable)
		{
			return;//전투 입력 불가상태에선 새로운 입력만 무시
		}

		if (actionInputType == EActionInputType::AIT_WeakAttack)
		{
			if (CheckAndTryGrap())
			{
				return;
			}
		}

		if (CurrentPlayActionData) StopAnimMontage(CurrentPlayActionData->AnimMontage); // 현재 재생중인 모션 정지
		if (!CheckConsumeableResource(playMotionData->ConsumeMana)) return;
	}

	SetCrouchState(false);
	if (playMotionData->ConsumeStemina > 0) CanRestoreStemina = false;

	if (playMotionData->AnimMontage)
	{
		ExcuteActionMotionByType(playMotionData, Trigger);
	}

	if (CurrentWeapon) // 현재 장착 무기 기준으로 액션을 실행
	{
		switch (actionInputType)
		{
		case EActionInputType::AIT_WeakAttack:
			CurrentWeapon->WeakAttackProcess(Trigger);
			break;
		case EActionInputType::AIT_StrongAttack:
			CurrentWeapon->StrongAttackProcess(Trigger);
			break;
		case EActionInputType::AIT_WeaponAction:
			CurrentWeapon->WeaponActionProcess(Trigger);
			break;
		case EActionInputType::AIT_WeaponSkill:
			CurrentWeapon->WeaponSkillProcess(Trigger);
			break;
		case EActionInputType::AIT_Avoid:
			CurrentWeapon->AvoidProcess(Trigger);
			break;
		default: break;
		}
	}
}

bool ACharacterDefaultBase::ExcuteActionMotionByType(UDA_ActionData* actionData, ETriggerEvent Trigger)
{
	if (IsDead) return false;

	if (Trigger == ETriggerEvent::Started)
	{
		SetCurrentToughness(MaxState.Toughness);
	}

	switch (actionData->ActionType)
	{
	case EActionType::AT_Instance:
		return ExcuteTypeInstance(actionData, Trigger);
		break;
	case EActionType::AT_Release:
		return ExcuteTypeRelease(actionData, Trigger);
		break;
	case EActionType::AT_Avoid:
		return ExcuteTypeAvoid(actionData, Trigger);
		break;
	case EActionType::AT_Charge:
		return ExcuteTypeCharge(actionData, Trigger);
		break;
	case EActionType::AT_Hold:
		return ExcuteTypeHold(actionData, Trigger);
		break;
	}
	return false;
}



bool ACharacterDefaultBase::ExcuteTypeInstance(UDA_ActionData* actionData, ETriggerEvent Trigger)
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
	return false;
}
bool ACharacterDefaultBase::ExcuteTypeRelease(UDA_ActionData* actionData, ETriggerEvent Trigger)
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
	return false;
}
bool ACharacterDefaultBase::ExcuteTypeAvoid(UDA_ActionData* actionData, ETriggerEvent Trigger)
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
			return true;
		}
		break;
	default: break;
	}
	return false;
}
bool ACharacterDefaultBase::ExcuteTypeCharge(UDA_ActionData* actionData, ETriggerEvent Trigger)
{
	switch (Trigger)
	{
	case ETriggerEvent::Started:
	{
		CurrentPlayActionData = actionData;
		PlayAnimMontage(actionData->AnimMontage, AnimMontagePlaySpeed, "Default");
		ReadyToAction();
		ChargeData.ChargeStep = 0;
		ChargeData.Started = true;
		ChargeData.ElaseTime = 0;
		ReadyToAction();
		return true;
	}
	break;
	case ETriggerEvent::Triggered:
	{
		if (ChargeData.ChargeStep < actionData->MaxChargeStep && ChargeData.ElaseTime / actionData->ChargeStepInterval >= ChargeData.ChargeStep + 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("차지단계 : %d"), ChargeData.ChargeStep);
			ChargeData.ChargeStep++;
			CurrentWeapon->SetCurrentMotionMulply(actionData->ChargeMotionMutiplyCurve->GetFloatValue(ChargeData.ChargeStep));
			UE_LOG(LogTemp, Warning, TEXT("모션배율 : %f"), actionData->ChargeMotionMutiplyCurve->GetFloatValue(ChargeData.ChargeStep));
		}
		return true;
	}
	break;
	case ETriggerEvent::Completed:
	{
		ChargeData.Started = false;
		FName CurrentSection = GetCurrentSection(actionData->AnimMontage);

		if (CurrentSection == "Default")
		{
			StopAnimMontage(actionData->AnimMontage);
			ActionEnd();
		}
		else if (CurrentSection == "Loop" || CurrentSection == "Charging")
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
	return false;
}
bool ACharacterDefaultBase::ExcuteTypeHold(UDA_ActionData* actionData, ETriggerEvent Trigger)
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
		FName CurrentSection = GetCurrentSection(actionData->AnimMontage); 
		if (CurrentSection == "Loop")
		{
			ReadyToAction();
			CurrentPlayActionData = actionData;
			AnimInstance->Montage_JumpToSection("Completed");
			return true;
		}
		else
		{
			StopAnimMontage(actionData->AnimMontage);
			ActionEnd();
		}
	}
	break;
	default: break;
	}
	return false;
}


bool ACharacterDefaultBase::CheckHostileChar(ACharacterDefaultBase* TargetChar, bool IsCheckNeutral)
{
	if (TargetChar->GetGenericTeamId() == GetGenericTeamId())
	{
		return false; // 같은팀이다
	}
	else if ((TargetChar->GetGenericTeamId()) == static_cast<uint8>(ETeam::T_Neutral))
	{
		return (IsCheckNeutral ?  true :  false);
	}
	return true;
}

void ACharacterDefaultBase::SetStun()
{
	if (DefaultMotion) PlayAnimMontage(DefaultMotion->StunMotion, 1);
	SetIsGrabable(true);
}

void ACharacterDefaultBase::SetIsGrabable(bool param)
{
	ACharacterPlayableBase* temp = Cast< ACharacterPlayableBase>(this);
	if (temp)
	{
		if (param)
		{
			UE_LOG(LogTemp, Warning, TEXT("셋 그랩에이블 true"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("셋 그랩에이블 false"));
		}
	}
	IsGrabable = param;
	if (GrapComp)
	{
		GrapComp->SetGrapReady(param);
	}
}


bool ACharacterDefaultBase::CheckAndTryGrap()
{
	TArray<FOverlapResult> HitResults;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(100);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->OverlapMultiByChannel(
		HitResults,
		GetActorLocation() + (GetActorForwardVector() * 50),
		FQuat::Identity,
		ECC_Pawn,
		Sphere,
		Params
	);

	UE_LOG(LogTemp, Warning, TEXT("1"));
	if (bHit)
	{
		for (const FOverlapResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (!HitActor)
				continue;
			if (ACharacterDefaultBase* target = Cast<ACharacterDefaultBase>(HitActor))
			{
				UE_LOG(LogTemp, Warning, TEXT("2"));
				return target->TriedGrab(this);
			}
		}
	}
	DrawDebugSphere(GetWorld(), GetActorLocation() + (GetActorForwardVector() * 50), 100, 16, FColor::Green, false, 0.05f);

	return false;
}
bool ACharacterDefaultBase::TriedGrab(ACharacterDefaultBase * Attacker)
{
	if (IsGrabable)
	{
		UE_LOG(LogTemp, Warning, TEXT("3"));
		return GrapComp->CheckGrabProcess(Attacker);
	}
	return false;
}


void ACharacterDefaultBase::ResetStates()
{
	IsDead = false;
	ActionEnd();
	CurrentState.HP = MaxState.HP;
	CurrentState.Mana = MaxState.Mana;
	CurrentState.Stemina = MaxState.Stemina;
	CurrentState.Toughness = MaxState.Toughness;
	CanRestoreStemina = true;
}


void ACharacterDefaultBase::PlaySound(FName SoundName , bool constance)
{
	USoundBase* sb_Sound;
	if (!DefaultSound) return;
	if (DefaultSound->SoundEffectMap.Contains(SoundName))
	{
		sb_Sound = *DefaultSound->SoundEffectMap.Find(SoundName);
		if (constance)
		{
			if (AudioComp->GetSound() != sb_Sound)
			{
				AudioComp->SetSound(sb_Sound);
				AudioComp->Play();
			}
		}
		else
		{
			AudioEffectComp->SetSound(sb_Sound);
			AudioEffectComp->Play();
		}

	}
}

void ACharacterDefaultBase::StopSound(bool constance)
{
	if (constance)
	{
		AudioComp->Stop();
		AudioComp->SetSound(NULL);
	}
	else
	{
		AudioEffectComp->Stop();
		AudioEffectComp->SetSound(NULL);
	}
}