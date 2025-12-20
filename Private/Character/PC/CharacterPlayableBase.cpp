// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPlayableBase.h"
#include "WeaponDefaultBase.h"
#include "LikeSoulLikeType.h"
#include "DA_ActionData.h"
#include "AnimInstanceDefaultBase.h"
#include "Manager_UI.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Manager_Calculate.h"
#include "DA_DefaultMotion.h"
#include "SoulLikeController.h"

// Sets default values
ACharacterPlayableBase::ACharacterPlayableBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterPlayableBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentResist;
	IsCrouch = false;
	FRotator rotation = Controller->GetControlRotation();
	FRotator yawRotation(0, rotation.Yaw, 0);
	LastInputDir = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y) * -1;

	CharacterCamp = ECharacterCamp::CC_Player;

	CameraComp = FindComponentByClass<UCameraComponent>();
	PlayerController = Cast<ASoulLikeController>(GetController());

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	CurrentWeaponSlot = 0;
	AddWeaponToMainSlot(Sword, 3);
	AddWeaponToMainSlot(Spear, 2);
	AddWeaponToMainSlot(Bow,   1);


	UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UManager_UI>();
	UIManager->InitUI(this);
	UpdateStatus();
}

// Called every frame
void ACharacterPlayableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckStatusCondition(DeltaTime);

	if (IsLockOn && LockOnTargetChar)
	{
		if (CheckContinueLockOn())
		{
			UpdateLockOnRotation(DeltaTime);
		}
		else
		{
			ReleaseLockOn();
		}
	}

	if (AnimInstance->GetSlotMontageGlobalWeight(FName("DefaultSlot")))
	{
		if (CurrentWeapon->bIsTwoHanded)
		{
			AnimInstance->SetIsTwoHandedWeapon(false);
		}
		CurrentState.Toughness = 1;
	}
	else
	{
		if (CurrentWeapon->bIsTwoHanded)
		{
			AnimInstance->SetIsTwoHandedWeapon(true);
		}
	}
}

// Called to bind functionality to input
void ACharacterPlayableBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterPlayableBase::HandleInput(FName ActionName, ETriggerEvent Trigger, const FInputActionInstance& instance)
{
	if (ActionName == "Move")				Move(Trigger, instance);
	else if (ActionName == "Look")			Look(Trigger, instance);
	else if (ActionName == "Avoid")			Avoid(Trigger, instance);

	else if (ActionName == "Interact")		Interact(Trigger);
	else if (ActionName == "UseItem")		UseItem(Trigger);
	else if (ActionName == "WeaponChange")	WeaponChange(Trigger);
	else if (ActionName == "LockOn")		LockOn(Trigger);

	else if (ActionName == "WeakAttack")	ExcutingWeaponBaseAction(EActionInputType::AIT_WeakAttack, Trigger, instance);
	else if (ActionName == "StrongAttack")	ExcutingWeaponBaseAction(EActionInputType::AIT_StrongAttack, Trigger, instance);
	else if (ActionName == "WeaponAction")	ExcutingWeaponBaseAction(EActionInputType::AIT_WeaponAction, Trigger, instance);
	else if (ActionName == "WeaponSkill")	ExcutingWeaponBaseAction(EActionInputType::AIT_WeaponSkill, Trigger, instance);
	else if (ActionName == "UserMenu")      UserMenu(Trigger);
	

}
void ACharacterPlayableBase::UserMenu(ETriggerEvent trigger)
{
	if (trigger == ETriggerEvent::Started)
	{
		UIManager->UserMenuKeyAction();
	}
}

void ACharacterPlayableBase::LockOn(ETriggerEvent Trigger)
{
	switch (Trigger)
	{
		case ETriggerEvent::Started:
		{
			if (IsLockOn)
			{
				ReleaseLockOn();
			}
			else
			{
				TryLockOn();
			}
		}
		break;
	default: break;
	}
}



void ACharacterPlayableBase::TryLockOn() 
{
	TArray<FOverlapResult> Overlaps;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(1000);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->OverlapMultiByChannel(
		Overlaps,
		GetActorLocation(),
		FQuat::Identity,
		ECC_Pawn,
		Sphere,
		Params
	);
	ACharacterDefaultBase* lockOnTarget = nullptr;
	float ClosestDistSq = 3000* 3000;

	for (auto& Target : Overlaps)
	{
		ACharacterDefaultBase* CharacterBase = Cast<ACharacterDefaultBase>(Target.GetActor());
		if (!CharacterBase || CharacterBase->GetIsDead()) continue;
		if (CheckTargetLock(CharacterBase, ClosestDistSq))
		{
			lockOnTarget = CharacterBase;
		}
	}
	if (lockOnTarget)
	{
		IsLockOn = true;
		LockOnTargetChar = lockOnTarget;
		LockOnTargetChar->StartLockOnUI();
		bUseControllerRotationYaw = true;
		CharacterMovement->bOrientRotationToMovement = false;
	}
}
bool ACharacterPlayableBase::CheckTargetLock(ACharacterDefaultBase* Target , float & ClosestDistSq)
{
	if (Target == this) return false;
	if (Target->GetCharCamp() != CC_Monster) return false;
	if (Target->GetIsDead()) return false;
	FVector PlayerLoc = this->GetActorLocation();
	FVector CameraLoc   = CameraComp->GetComponentLocation();
	FVector TargetLoc = Target->GetActorLocation();

	//거리 체크 지금까지 가장 가까웠던 거리와 체크
	float DistSq = FVector::DistSquared(PlayerLoc, TargetLoc);


	if (DistSq > ClosestDistSq) return false;

	//시야각 1차 체크
	FVector ToTarget = (TargetLoc - CameraLoc).GetSafeNormal();
	float Dot = FVector::DotProduct(CameraComp->GetForwardVector(), ToTarget);
	if (Dot < 0.6f) return false;

	//화면 투영되어있는지 체크
	FVector2D ScreenPos;
	bool bOnScreen = UGameplayStatics::ProjectWorldToScreen(PlayerController, TargetLoc, ScreenPos);
	if (!bOnScreen) return false;

	//화면 해상도 체크
	int32 SizeX, SizeY;
	PlayerController->GetViewportSize(SizeX, SizeY);
	if (ScreenPos.X < 0 || ScreenPos.X > SizeX ||
		ScreenPos.Y < 0 || ScreenPos.Y > SizeY)
		return false;


	FHitResult Hit;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner()); // 플레이어
	Params.AddIgnoredActor(this);       // 컴포넌트 소유자
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		CameraLoc,
		TargetLoc,
		ECC_Visibility, Params
	);

	if (bHit && Hit.GetActor() != Target) return false;

	ClosestDistSq = DistSq;
	return true;
}
bool ACharacterPlayableBase::CheckContinueLockOn()
{
	if (LockOnTargetChar->GetIsDead()) return false;

	FVector PlayerLoc = this->GetActorLocation();
	FVector TargetLoc = LockOnTargetChar->GetActorLocation();
	float DistSq = FVector::DistSquared(PlayerLoc, TargetLoc);
	//거리 계산
	if (DistSq > 3000 * 3000) return false;
	//화면 투영되어있는지 체크
	FVector2D ScreenPos;
	bool bOnScreen = UGameplayStatics::ProjectWorldToScreen(PlayerController, TargetLoc, ScreenPos);
	if (!bOnScreen) return false;

	return true;
}

void ACharacterPlayableBase::ReleaseLockOn()
{
	IsLockOn = false;
	LockOnTargetChar->EndLockOnUI();
	CharacterMovement->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	LockOnTargetChar = nullptr;
}

void ACharacterPlayableBase::UpdateLockOnRotation(float DeltaTime)
{
	FVector CameraLoc = CameraComp->GetComponentLocation();
	FVector TargetLoc = LockOnTargetChar->GetMesh()->GetSocketLocation(TEXT("Death_Torso"));
	TargetLoc.Z -= 60;
	FRotator DesiredRot = UKismetMathLibrary::FindLookAtRotation(CameraLoc, TargetLoc);

	FRotator CurrentRot = GetController()->GetControlRotation();

	FRotator NewRot = FMath::RInterpTo(CurrentRot,DesiredRot,DeltaTime,5.0f);

	GetController()->SetControlRotation(NewRot);
}


void ACharacterPlayableBase::Move(ETriggerEvent Trigger, const FInputActionInstance& Value)
{

	switch (Trigger)
	{
		case ETriggerEvent::Started:
			{
			}
			break;
		case ETriggerEvent::Triggered:
			{
				//멈춤상태에서 구르기 방향 입력을 위해 방향은 초기화
				FRotator rotation = Controller->GetControlRotation();
				rotation.Pitch = 0.f;
				rotation.Roll = 0.f;
				FRotator yawRotation(0, rotation.Yaw, 0);

				FVector FrontDir = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
				FVector RightDir = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
				FrontDir.Z = 0;
				RightDir.Z = 0;

				FVector2D tempVector = Value.GetValue().Get<FVector2D>();
				LastInputDir = (FrontDir * tempVector.Y) + (RightDir * tempVector.X);
				LastInputDir.Z = 0;
				LastInputDir.Normalize();

				if (!IsMoveable) return;

				CharacterMovement->bOrientRotationToMovement = true;
				CharacterMovement->RotationRate = FRotator(0, 540, 0);

				if (IsRunning)
				{
					if (IsLockOn)
					{
						CharacterMovement->bOrientRotationToMovement = true;
						bUseControllerRotationYaw = false;
					}
					ModifyCurrentStemina(-1);
					if (CurrentState.Stemina <= 0) IsRunning = false;
				}
				else
				{
					if (IsLockOn)
					{
						CharacterMovement->bOrientRotationToMovement = false;
						bUseControllerRotationYaw = true;
					}
					tempVector *= 0.6f;
					tempVector = IsCrouch ? tempVector *= 0.5f : tempVector;
				}
				if (IsMoveable)
				{
					AddMovementInput(FrontDir, tempVector.Y);
					AddMovementInput(RightDir, tempVector.X);
				}
			}
			break;
		case ETriggerEvent::Completed:
			{
				LastInputDir = FVector::ZeroVector;
			}
		break;
		default: break;
	}
}

void ACharacterPlayableBase::Look(ETriggerEvent trigger, const FInputActionInstance& Value)
{
	if (IsLockOn) return;
	switch (trigger)
	{
		case ETriggerEvent::Triggered:
		{
			bUseControllerRotationYaw = false;  // 캐릭터가 컨트롤러 회전에 따라가지 않도록
			FVector2D temp = Value.GetValue().Get<FVector2D>();
			AddControllerYawInput(temp.X);
			AddControllerPitchInput(temp.Y);
		}
		break;
	default: break;
	}
}

void ACharacterPlayableBase::Avoid(ETriggerEvent trigger, const FInputActionInstance& Value)
{
	if (!IsAvoidable) return;
	switch (trigger)
	{
		case ETriggerEvent::Started:
		{
			UE_LOG(LogTemp, Warning, TEXT("started"));
		}
		break;
		case ETriggerEvent::Triggered:
		{
			if (IsRunable && Value.GetElapsedTime() > 0.2f && CurrentState.Stemina > 0)
			{
				SetCrouchState(false);
				IsRunning = true;
			}
		}
		break;
		case  ETriggerEvent::Completed:
		{
			if (IsRunning)
			{
				UE_LOG(LogTemp, Warning, TEXT("RunningEnd"));
				IsRunning = false;
			}
			else if (LastInputDir == FVector::ZeroVector)
			{
				UE_LOG(LogTemp, Warning, TEXT("Crouch"));
				SetCrouchState(!IsCrouch);
			}
			else
			{
				FRotator TargetRot = Controller->GetControlRotation();
				TargetRot.Pitch = TargetRot.Roll = 0;
				SetActorRotation(TargetRot);
				ExcutingWeaponBaseAction(EActionInputType::AIT_Avoid, trigger, Value);
			}
		}
		break;
	default: break;
	}

}
void ACharacterPlayableBase::Interact(ETriggerEvent Trigger)
{
	if (!IsMoveable) return;
	//근처의 상호작용 오브젝트 찾기 .. 충돌로?


	//상호 작용 오브젝트가 있으면 상호작용 오브젝트의 함수를 실행한다
	switch (Trigger)
	{
	case ETriggerEvent::Started:
	{
	}
	break;
	case ETriggerEvent::Triggered:
	{
	}
	break;
	case ETriggerEvent::Completed:
	{
	}
	break;
	default: break;
	}
}

void ACharacterPlayableBase::UseItem(ETriggerEvent Trigger)
{
	//퀵슬롯에 올려져있는 아이템 바로 사용
	if (!IsMoveable) return;
	SetCrouchState(false);
}
void ACharacterPlayableBase::WeaponChange(ETriggerEvent trigger)
{
	if (!IsMoveable) return;
	switch (trigger)
	{
	case ETriggerEvent::Started:
	{
		EqiupMainSlotWeapon((CurrentWeaponSlot + 1) % 4);
	}
	break;
	default: break;
	}
}

void ACharacterPlayableBase::ExcutingWeaponBaseAction(EActionInputType actionInputType, ETriggerEvent Trigger, const FInputActionInstance& instance)
{
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
	if (CurrentState.Stemina <= 0) return;
	if (CurrentState.Mana < playMotionData->ConsumeMana) return;
	if (Trigger == ETriggerEvent::Started)
	{
		if (!IsAttackable) return;//전투 입력 불가상태에선 새로운 입력만 무시
		if(CurrentPlayActionData) StopAnimMontage(CurrentPlayActionData->AnimMontage); // 현재 재생중인 모션 정지
	}

	SetCrouchState(false);
	if (playMotionData->ConsumeStemina > 0) CanRestoreStemina = false;

	if (playMotionData->AnimMontage)
	{
		if (ExcuteActionMotionByType(playMotionData, Trigger, instance))  CurrentPlayActionData = playMotionData;
	}

	if (CurrentWeapon) // 현재 장착 무기 기준으로 액션을 실행
	{
		switch (actionInputType)
		{
			case EActionInputType::AIT_WeakAttack:
				CurrentWeapon->WeakAttackProcess( Trigger, instance);
				break;
			case EActionInputType::AIT_StrongAttack:
				CurrentWeapon->StrongAttackProcess( Trigger, instance);
				break;
			case EActionInputType::AIT_WeaponAction:
				CurrentWeapon->WeaponActionProcess( Trigger, instance);
				break;
			case EActionInputType::AIT_WeaponSkill:
				CurrentWeapon->WeaponSkillProcess( Trigger, instance);
				break;
			case EActionInputType::AIT_Avoid:
				CurrentWeapon->AvoidProcess( Trigger, instance);
				break;
			default: break;
		}
	}
}

void ACharacterPlayableBase::CalculateResist()
{

}

void ACharacterPlayableBase::SetStatusCondition(FStatusCondition condition)
{
	FStatusCondition* FoundCondition = FindStatusByName(condition.ConditionName);
	if (FoundCondition)
	{
		FoundCondition->DurationTime = condition.DurationTime;
	}
	else
	{
		arrStatusCondition.Add(condition);
	}
}
FStatusCondition* ACharacterPlayableBase::FindStatusByName(FName name)
{
	return arrStatusCondition.FindByPredicate([name](const FStatusCondition& W)
		{
			return ((W.ConditionName == name));
		});
}


void ACharacterPlayableBase::CheckStatusCondition(float DeltaTime)
{
	for (int i = arrStatusCondition.Num() - 1; i >= 0; --i)
	{
		FStatusCondition& Effect = arrStatusCondition[i];
		if (Effect.bIsPermanent) continue;
		Effect.DurationTime -= DeltaTime;
		if (Effect.DurationTime <= 0.0f) arrStatusCondition.RemoveAt(i);
	}
}


void ACharacterPlayableBase::DiedProcess()
{
	IsDead = true;
	UIManager->SetDeadUI();
	AnimInstance->StopAllMontages(0);
	PlayAnimMontage(DefaultMotion->DiedMotion, 1);
}


void ACharacterPlayableBase::AddWeaponToMainSlot(TSubclassOf<AWeaponDefaultBase> weapon, int slotNum)
{
	if (!weapon)return;
	if (slotNum > 3) return;
	AWeaponDefaultBase* TempWeapon = GetWorld()->SpawnActor<AWeaponDefaultBase>(weapon, FTransform());

	if (TempWeapon)
	{
		if (MainWeaponSlot[slotNum])
		{
			MainWeaponSlot[slotNum]->Destroy();
			MainWeaponSlot[slotNum] = nullptr;
		}

		MainWeaponSlot[slotNum] = TempWeapon;

		MainWeaponSlot[slotNum]->SettingWeaponLocation(this);
		if (slotNum != CurrentWeaponSlot)
		{
			MainWeaponSlot[slotNum]->SetActiveWeapon(false);
		}
		else
		{
			EqiupMainSlotWeapon(slotNum);
		}
	}
}

void ACharacterPlayableBase::EqiupMainSlotWeapon(int slotNum)
{

	if (slotNum > 3) return;

	if (MainWeaponSlot[slotNum])
	{
		if (CurrentWeapon)
		{
			CurrentWeapon->SetActiveWeapon(false);
		}
		MainWeaponSlot[slotNum]->SetActiveWeapon(true);
		CurrentWeaponSlot = slotNum;
		CurrentWeapon = MainWeaponSlot[slotNum];



		if (!CurrentWeapon->bIsTwoHanded)
		{
			AnimInstance->SetIsTwoHandedWeapon(false);
		}

		if (CurrentWeapon->IdleMotion)  AnimInstance->WeaponIdleMotion = CurrentWeapon->IdleMotion;
		if (CurrentWeapon->WalkMotion)	AnimInstance->WeaponWalkMotion = CurrentWeapon->WalkMotion;
		if (CurrentWeapon->RunMotion)	AnimInstance->WeaponRunMotion = CurrentWeapon->RunMotion;
	}
}


void ACharacterPlayableBase::ModifyCurrentHealth(int param)
{
	Super::ModifyCurrentHealth(param);
	UIManager->SetPlayerHP(CurrentState.HP, MaxState.HP);
	
}
void ACharacterPlayableBase::ModifyCurrentStemina(int stemina)
{
	Super::ModifyCurrentStemina(stemina);
	UIManager->SetPlayerSP(CurrentState.Stemina, MaxState.Stemina);

}
void ACharacterPlayableBase::ModifyCurrentMana(int mana)
{
	Super::ModifyCurrentMana(mana);
	if (GetWorld()->GetFirstPlayerController()->GetPawn() == this)
	{
		UIManager->SetPlayerMP(CurrentState.Mana, MaxState.Mana);
	}
}

void ACharacterPlayableBase::UpdateStatus()
{
	MaxState = CalculatManager->CalculateMaxState(CurrentStatus);
	CurrentState = MaxState;

	UIManager->SetPlayerMaxHP(MaxState.HP);
	UIManager->SetPlayerMaxMP(MaxState.Mana);
	UIManager->SetPlayerMaxSP(MaxState.Stemina);

	UIManager->SetPlayerHP(CurrentState.HP, MaxState.HP);
	UIManager->SetPlayerMP(CurrentState.Mana, MaxState.Mana);
	UIManager->SetPlayerSP(CurrentState.Stemina, MaxState.Stemina);

	UE_LOG(LogTemp, Warning, TEXT("업데이트 스테이트 %d"), MaxState.Stemina);

}