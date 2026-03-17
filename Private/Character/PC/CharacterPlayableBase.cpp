// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPlayableBase.h"
#include "WeaponDefaultBase.h"
#include "LikeSoulLikeType.h"
#include "DA_ActionData.h"
#include "AnimInstanceDefaultBase.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Manager_Calculate.h"
#include "Manager_UI.h"
#include "DA_DefaultMotion.h"
#include "SoulLikeController.h"
#include "InventoryComponent.h"
#include "DropItem.h"
#include "Manager_ItemInfo.h"
#include "BoneFire.h"
#include "Manager_Bonefire.h"
#include "Manager_Enemy.h"
#include "LockOnComponent.h"
#include "InteractComponent.h"
#include "EquipmentComponent.h"
#include "PlayerStatusComponent.h"
#include "DeathRespawnComponent.h"
#include "ItemUseComponent.h"


// Sets default values
ACharacterPlayableBase::ACharacterPlayableBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	LockOnComp = CreateDefaultSubobject<ULockOnComponent>(TEXT("LockOnComponent"));
	InteractComp = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
	EquipmentComp = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));
	PlayerStatusComp = CreateDefaultSubobject<UPlayerStatusComponent>(TEXT("PlayerStatusComponent"));
	DeathRespawnComp = CreateDefaultSubobject<UDeathRespawnComponent>(TEXT("DeathRespawnComponent"));
	ItemUseComp = CreateDefaultSubobject<UItemUseComponent>(TEXT("ItemUseComponent"));
}

FGenericTeamId ACharacterPlayableBase::GetGenericTeamId() const
{
	if (PlayerController)
	{
		return FGenericTeamId(PlayerController->GetGenericTeamId());
	}
	return FGenericTeamId(static_cast<uint8>(ETeam::T_Neutral));
}

// Called when the game starts or when spawned
void ACharacterPlayableBase::BeginPlay()
{
	Super::BeginPlay();


	IsCrouch = false;
	FRotator rotation = Controller->GetControlRotation();
	FRotator yawRotation(0, rotation.Yaw, 0);
	LastInputDir = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y) * -1;

	CameraComp = FindComponentByClass<UCameraComponent>();
	SpringArmComp = FindComponentByClass<USpringArmComponent>();
	InventoryComp->SetInventoryOwner(this);
	PlayerController = Cast<ASoulLikeController>(GetController());
	if (LockOnComp)
	{
		LockOnComp->Initialize(this, CameraComp, PlayerController, CharacterMovement);
	}
	if (InteractComp)
	{
		InteractComp->Initialize(this);
	}
	if (EquipmentComp)
	{
		EquipmentComp->Initialize(this);
	}
	if (PlayerStatusComp)
	{
		PlayerStatusComp->Initialize(this);
	}
	if (DeathRespawnComp)
	{
		DeathRespawnComp->Initialize(this);
	}
	if (ItemUseComp)
	{
		ItemUseComp->Initialize(this);
	}

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UManager_UI>();
	UIManager->InitUI(this);
	UpdateStatus();
	if (PlayerStatusComp)
	{
		PlayerStatusComp->BroadcastAll();
	}
	CurrentWeaponSlotNum = 0;
	CurrentQuickSlotNum  = 0;

	ItemInfoManager = GetGameInstance()->GetSubsystem<UManager_ItemInfo>();

	if (EquipmentComp)
	{
		EquipmentComp->AddWeaponToMainSlot(BaseWeapon, BaseWeaponSlot);
		EquipmentComp->EquipMainSlotWeapon(0);
	}
	else
	{
		AddWeaponToMainSlot(BaseWeapon, BaseWeaponSlot);
		EqiupMainSlotWeapon(0);
	}

	FItemData temp = InventoryComp->GetWeaponSlotWeapon(CurrentWeaponSlotNum);
	UIManager->ChangeWeaponSlot(temp);

	temp = InventoryComp->GetQuickSlotItem(CurrentWeaponSlotNum);
	UIManager->ChangeQuickSlot(temp);

	GetGameInstance()->GetSubsystem<UManager_Enemy>()->UpdatePlayerCharacter(this);

	BonefireManager = GetWorld()->GetGameInstance()->GetSubsystem<UManager_Bonefire>();
	
}
void ACharacterPlayableBase::UpdateCurrentWeaponSlotUI()
{
	if (EquipmentComp)
	{
		EquipmentComp->UpdateCurrentWeaponSlotUI();
		return;
	}
	FItemData temp = InventoryComp->GetWeaponSlotWeapon(CurrentWeaponSlotNum);
	UIManager->ChangeWeaponSlot(temp);
}
void ACharacterPlayableBase::UpdateCurrentQuickSlotUI()
{
	if (EquipmentComp)
	{
		EquipmentComp->UpdateCurrentQuickSlotUI();
		return;
	}
	FItemData temp = InventoryComp->GetQuickSlotItem(CurrentQuickSlotNum);
	UIManager->ChangeQuickSlot(temp);
}

UManager_Enemy* ACharacterPlayableBase::GetEnemyManager() const
{
	return GetGameInstance()->GetSubsystem<UManager_Enemy>();
}

void ACharacterPlayableBase::SetLockOnState(bool bOn, ACharacterDefaultBase* Target)
{
	IsLockOn = bOn;
	LockOnTargetChar = Target;
}

void ACharacterPlayableBase::SetCurrentWeaponInternal(AWeaponDefaultBase* NewWeapon)
{
	CurrentWeapon = NewWeapon;
}

void ACharacterPlayableBase::SetDeadStateInternal(bool bDead)
{
	IsDead = bDead;
}

void ACharacterPlayableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckStatusCondition(DeltaTime);

	if (LockOnComp)
	{
		LockOnComp->TickLockOn(DeltaTime);
	}

	if (CurrentWeapon)
	{

		if (CurrentWeapon->bIsTwoHanded)
		{
			UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
			if (AnimInstance->GetSlotMontageGlobalWeight(FName("DefaultSlot")) || (CurrentMontage && CurrentMontage->GetName() == "UsePotion_Montage"))
			{
				AnimInstance->SetIsTwoHandedWeapon(false);
			}
			else
			{
				AnimInstance->SetIsTwoHandedWeapon(true);
			}
		}

	}
}

void ACharacterPlayableBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterPlayableBase::HandleInput(FName ActionName, ETriggerEvent Trigger, const FInputActionInstance& instance)
{
	if (ActionName == "Move")				Move(Trigger, instance);
	else if (ActionName == "Look")			Look(Trigger, instance);
	else if (ActionName == "Avoid")			Avoid(Trigger, instance);

	else if (ActionName == "Interact")			Interact(Trigger);
	else if (ActionName == "UseItem")			UseItem(Trigger);
	else if (ActionName == "WeaponChange")		WeaponChange(Trigger);
	else if (ActionName == "QuickSlotChange")	QuickSlotChange(Trigger);
	else if (ActionName == "LockOn")			LockOn(Trigger);

	else if (ActionName == "WeakAttack")	ExcutingWeaponBaseAction(EActionInputType::AIT_WeakAttack, Trigger);
	else if (ActionName == "StrongAttack")	ExcutingWeaponBaseAction(EActionInputType::AIT_StrongAttack, Trigger);
	else if (ActionName == "WeaponAction")	ExcutingWeaponBaseAction(EActionInputType::AIT_WeaponAction, Trigger);
	else if (ActionName == "WeaponSkill")	ExcutingWeaponBaseAction(EActionInputType::AIT_WeaponSkill, Trigger);
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
			LockOnComp->ToggleLockOn();
		}
		break;
	default: break;
	}
}



void ACharacterPlayableBase::Move(ETriggerEvent Trigger, const FInputActionInstance& Value)
{
	const bool bLockedOn = LockOnComp->IsLockedOn();

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

				if (!IsMoveable || IsAimMode)
				{
					StopSound(true);
					return;
				}

				CharacterMovement->bOrientRotationToMovement = true;
				CharacterMovement->RotationRate = FRotator(0, 540, 0);

				if (IsRunning)
				{
					if (bLockedOn)
					{
						CharacterMovement->bOrientRotationToMovement = true;
						bUseControllerRotationYaw = false;
					}					
					tempVector *= 1.2f;
					SprintStaminaAcc += Value.GetElapsedTime();

					if ((Value.GetElapsedTime()- LastElapsedInterval) > 0.1f )
					{
						LastElapsedInterval = Value.GetElapsedTime();
						ModifyCurrentStemina(-1);

						if (CurrentState.Stemina <= 0)
						{
							IsRunning = false;
						}
					}
					
					if (CurrentState.Stemina <= 0) IsRunning = false;
					PlaySound("Run",true);
				}
				else
				{
					if (bLockedOn)
					{
						CharacterMovement->bOrientRotationToMovement = false;
						bUseControllerRotationYaw = true;
					}
					tempVector *= 0.6f;
					tempVector = IsCrouch ? tempVector *= 0.5f : tempVector;
					PlaySound("Walk", true);
				}

				AddMovementInput(FrontDir, tempVector.Y);
				AddMovementInput(RightDir, tempVector.X);
			}
			break;
		case ETriggerEvent::Completed:
			{
				LastElapsedInterval = 0;
				LastInputDir = FVector::ZeroVector;
				StopSound(true);
			}
		break;
		default: break;
	}
}

void ACharacterPlayableBase::Look(ETriggerEvent trigger, const FInputActionInstance& Value)
{
	if (LockOnComp->IsLockedOn()) return;
	switch (trigger)
	{
		case ETriggerEvent::Triggered:
		{
			bUseControllerRotationYaw = false; 
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
				IsRunning = false;
			}
			else if (LastInputDir == FVector::ZeroVector)
			{
				SetCrouchState(!IsCrouch);
			}
			else
			{
				FRotator TargetRot = Controller->GetControlRotation();
				TargetRot.Pitch = TargetRot.Roll = 0;
				SetActorRotation(TargetRot);
				ExcutingWeaponBaseAction(EActionInputType::AIT_Avoid, trigger);
			}
		}
		break;
	default: break;
	}

}

void ACharacterPlayableBase::SetAimingMode(bool On)
{
	if (On)
	{
		GetAnimInstance()->SetAimOffsetMode(true);
		SetIsMoveable(false);
		IsAimMode = true;
		SpringArmComp->TargetArmLength = 60;
		SpringArmComp->SocketOffset.Y = 40;
		OnAimWidgetChanged.Broadcast(true);
	}
	else
	{
		IsAimMode = false;
		SpringArmComp->TargetArmLength = 200;
		SpringArmComp->SocketOffset.Y = 0;
		GetAnimInstance()->SetAimOffsetMode(false);
		GetAnimInstance()->StopAllMontages(0);
		OnAimWidgetChanged.Broadcast(false);
		ActionEnd();
	}
}
void ACharacterPlayableBase::Interact(ETriggerEvent Trigger)
{
	if (!IsMoveable) return;
	//상호 작용 오브젝트가 있으면 상호작용 오브젝트의 함수를 실행한다
	switch (Trigger)
	{
		case ETriggerEvent::Started:
		{
			InteractComp->HandleInteract();
		}
		break;
	default: break;
	}
}

void ACharacterPlayableBase::UseItem(ETriggerEvent Trigger)
{
	//퀵슬롯에 올려져있는 아이템 바로 사용

	if (!IsMoveable) return; 
	switch (Trigger)
	{
		case ETriggerEvent::Started:
		{
			ItemUseComp->HandleUseItem();
		}
	}
}


void ACharacterPlayableBase::WeaponChange(ETriggerEvent trigger)
{
	if (!IsMoveable) return;
	switch (trigger)
	{
	case ETriggerEvent::Started:
	{
		EquipmentComp->WeaponChange();
	}
	break;
	default: break;
	}
}
void ACharacterPlayableBase::QuickSlotChange(ETriggerEvent trigger)
{
	switch (trigger)
	{
		case ETriggerEvent::Started:
		{
			EquipmentComp->QuickSlotChange();
		}
		break;
		default:break;
	}
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
	for (int32 i = arrStatusCondition.Num() - 1; i >= 0; --i)
	{
		FStatusCondition& Effect = arrStatusCondition[i];
		if (Effect.bIsPermanent) continue;
		Effect.DurationTime -= DeltaTime;
		if (Effect.DurationTime <= 0.0f) arrStatusCondition.RemoveAt(i);
	}
}


void ACharacterPlayableBase::DiedProcess()
{
	if (DeathRespawnComp)
	{
		DeathRespawnComp->HandleDeath();
		return;
	}
	IsDead = true;
	AnimInstance->StopAllMontages(0);
	PlayAnimMontage(DefaultMotion->DiedMotion, 1);
	UIManager->AnnouncementFadeOut("You Died");

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this]()
		{
			GetGameInstance()->GetSubsystem<UManager_Enemy>()->RespawnAll();
			DropSoul();
			BonefireManager->RespawnLastBonefire(this);
			ResetStates();
			InventoryComp->RefillInventory();
			UIManager->SetPlayerHP(CurrentState.HP, MaxState.HP);
			UIManager->SetPlayerSP(CurrentState.Stemina, MaxState.Stemina);
			UIManager->SetPlayerMP(CurrentState.Mana, MaxState.Mana);
			UIManager->ShowBossHPUI(false);
			FItemData temp = InventoryComp->GetQuickSlotItem(CurrentQuickSlotNum);
			UIManager->ChangeQuickSlot(temp);
			FTimerHandle handle2;
			GetWorld()->GetTimerManager().SetTimer(handle2, [this]()
				{
					UIManager->InvisibleAnnouncement();
				},
				1.0f,
				false);
		},
		3.0f,
		false);

}


void ACharacterPlayableBase::ModifyCurrentSoul(int32 soul)
{
	PlayerStatusComp->AddSoul(soul);
}


void ACharacterPlayableBase::DropSoul()
{
	TObjectPtr<ADropItem> TempWeapon = GetWorld()->SpawnActor<ADropItem>(DropSoulBP, FTransform());
	if (!TempWeapon) return;
	ADropItem * DropSoul = TempWeapon.Get();
	DropSoul->SetDropSoul(CurrentStatus.Soul);
	BonefireManager->SetDropSoul(DropSoul);
	ModifyCurrentSoul(CurrentStatus.Soul * -1);
	FVector Loc = GetActorLocation();
	Loc.Z -= 90;
	TempWeapon->SetActorLocation(Loc);
}

void ACharacterPlayableBase::AddWeaponToMainSlot(TSoftClassPtr<AWeaponDefaultBase> weapon, int32 slotNum)
{
	EquipmentComp->AddWeaponToMainSlot(weapon, slotNum);
	TSubclassOf<AWeaponDefaultBase> SpwanBp = weapon.Get(); // 로드 확인
	if (!SpwanBp) SpwanBp = weapon.LoadSynchronous(); //동기 로드 시도
	if (!SpwanBp) return; //동기 로드도 실패면 리턴
	if (slotNum > BaseWeaponSlot) return; //맨손 저장용으로 1개 늘어남

	TObjectPtr<AWeaponDefaultBase> TempWeapon = GetWorld()->SpawnActor<AWeaponDefaultBase>(SpwanBp, FTransform());
	if (TempWeapon)
	{
		if (MainWeaponSlot[slotNum])
		{
			MainWeaponSlot[slotNum]->Destroy();
			MainWeaponSlot[slotNum] = nullptr;
		}

		MainWeaponSlot[slotNum] = TempWeapon.Get();

		FItemData* Weapondata = InventoryComp->GetWeaponSlotWeaponInfo(slotNum);
		if (Weapondata)
		{
			MainWeaponSlot[slotNum]->SetItemData(Weapondata);
		}

		MainWeaponSlot[slotNum]->SettingWeaponData(this);
		if (slotNum != CurrentWeaponSlotNum)
		{
			MainWeaponSlot[slotNum]->SetActiveWeapon(false);
		}
		else
		{
			EqiupMainSlotWeapon(slotNum); 
		}
		FItemData temp = InventoryComp->GetWeaponSlotWeapon(slotNum);
		UIManager->ChangeWeaponSlot(temp);
	}
}

void ACharacterPlayableBase::ResetWeaponSlotInfo(int32 slotNum)
{
	EquipmentComp->ResetWeaponSlotInfo(slotNum);
	if (slotNum > BaseWeaponSlot-1) return;
	if (MainWeaponSlot[slotNum])
	{
		MainWeaponSlot[slotNum]->Destroy();
		MainWeaponSlot[slotNum] = nullptr;
		if(slotNum == CurrentWeaponSlotNum) EqiupMainSlotWeapon(slotNum);
	}
}

void ACharacterPlayableBase::EqiupQuickSlotItem(int32 slotNum)
{
	EquipmentComp->EquipQuickSlotItem(slotNum);
}
void ACharacterPlayableBase::EqiupMainSlotWeapon(int32 slotNum)
{
	EquipmentComp->EquipMainSlotWeapon(slotNum);
}


void ACharacterPlayableBase::ModifyCurrentHealth(int32 param)
{
	Super::ModifyCurrentHealth(param);
	PlayerStatusComp->OnHealthChanged();
	
}
void ACharacterPlayableBase::ModifyCurrentStemina(int32 stemina )
{
	Super::ModifyCurrentStemina(stemina);
	PlayerStatusComp->OnSteminaChanged();

}
void ACharacterPlayableBase::ModifyCurrentMana(int32 mana)
{
	Super::ModifyCurrentMana(mana);
	PlayerStatusComp->OnManaChanged();
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
}