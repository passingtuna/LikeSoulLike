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


// Sets default values
ACharacterPlayableBase::ACharacterPlayableBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
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

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UManager_UI>();
	UIManager->InitUI(this);
	UpdateStatus();
	CurrentWeaponSlotNum = 0;
	CurrentQuickSlotNum  = 0;

	ItemInfoManager = GetGameInstance()->GetSubsystem<UManager_ItemInfo>();

	AddWeaponToMainSlot(BaseWeapon, BaseWeaponSlot);
	EqiupMainSlotWeapon(0);

	FItemData temp = InventoryComp->GetWeaponSlotWeapon(CurrentWeaponSlotNum);
	UIManager->ChangeWeaponSlot(temp);

	temp = InventoryComp->GetQuickSlotItem(CurrentWeaponSlotNum);
	UIManager->ChangeQuickSlot(temp);
	UIManager->ChangeSoul(CurrentStatus.Soul);

	GetGameInstance()->GetSubsystem<UManager_Enemy>()->UpdatePlayerCharacter(this);

	BonefireManager = GetWorld()->GetGameInstance()->GetSubsystem<UManager_Bonefire>();
	
}
void ACharacterPlayableBase::UpdateCurrentWeaponSlotUI()
{
	FItemData temp = InventoryComp->GetWeaponSlotWeapon(CurrentWeaponSlotNum);
	UIManager->ChangeWeaponSlot(temp);
}
void ACharacterPlayableBase::UpdateCurrentQuickSlotUI()
{
	FItemData temp = InventoryComp->GetQuickSlotItem(CurrentQuickSlotNum);
	UIManager->ChangeQuickSlot(temp);
}

// Called every frame
void ACharacterPlayableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UIManager->ChangeSoul(CurrentStatus.Soul);
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


	//DrawDebugSphere(GetWorld(), GetActorLocation(), 1000, 16, FColor::Green, false, 0.05f);
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
	if (!CheckHostileChar(Target,false)) return false; //중립과 아군은 록온타겟에서 제외
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

	FRotator NewRot = FMath::RInterpTo(CurrentRot,DesiredRot,DeltaTime,20.0f);

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

				if (!IsMoveable || IsAimMode)
				{
					StopSound(true);
					return;
				}

				CharacterMovement->bOrientRotationToMovement = true;
				CharacterMovement->RotationRate = FRotator(0, 540, 0);

				if (IsRunning)
				{
					if (IsLockOn)
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
					if (IsLockOn)
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
				UE_LOG(LogTemp, Warning, TEXT("StopSound"));
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
	if (IsLockOn) return;
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
				UE_LOG(LogTemp, Warning, TEXT("Crouch"));
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
		UIManager->ShowAimWidget(true);
	}
	else
	{
		IsAimMode = false;
		SpringArmComp->TargetArmLength = 200;
		SpringArmComp->SocketOffset.Y = 0;
		UE_LOG(LogTemp, Warning, TEXT("해제"));
		GetAnimInstance()->SetAimOffsetMode(false);
		GetAnimInstance()->StopAllMontages(0);
		UIManager->ShowAimWidget(false);
		ActionEnd();
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
			TArray<FOverlapResult> Overlaps;

			FCollisionShape Sphere = FCollisionShape::MakeSphere(50);
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);

			FVector SearchLoc = GetActorLocation() + (GetActorForwardVector() * 20);

			SearchLoc.Z -= 50;
			GetWorld()->OverlapMultiByChannel(
				Overlaps, 
				SearchLoc,
				FQuat::Identity,
				ECC_Pawn,
				Sphere,
				Params
			);
			for (auto& Target : Overlaps)
			{
				ADropItem* DropItem = Cast<ADropItem>(Target.GetActor());
				if (DropItem)
				{
					Interact_Item(DropItem);
				}

				ABoneFire* Bonefire = Cast<ABoneFire>(Target.GetActor());
				if (Bonefire)
				{
					Interact_BoneFire(Bonefire);
				}
			}
			//DrawDebugSphere(GetWorld(), SearchLoc, 100, 16, FColor::Green, false, 0.05f);
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

void ACharacterPlayableBase::Interact_Item(ADropItem* DropItem)
{
	PlaySound("GetItem");
	if (DropItem->IsSoul)
	{
		ModifyCurrentSoul(DropItem->Soul);
		DropItem->Destroy();
		return;
	}
	else
	{
		FString tempString;
		FItemData tempItemCount = DropItem->ItemInfo;
		int32 result = InventoryComp->PutInItemToInventory(DropItem->ItemInfo);
		if (result <= 0)
		{
			DropItem->Destroy();
		}
		else //인벤 창고 꽉차서 필드에 남김
		{
			tempString = TEXT("가득참 : ");
			DropItem->ItemInfo.Count = result;
			tempItemCount.Count -= result;
		}
		tempString += GetGameInstance()->GetSubsystem<UManager_ItemInfo>()->GetItemNameString(tempItemCount);

		UIManager->ShowGetItemUI(DropItem->ItemInfo, FText::FromString(tempString));
		FItemData temp = InventoryComp->GetQuickSlotItem(CurrentQuickSlotNum);
		UIManager->ChangeQuickSlot(temp);
		return;
	}
}
void ACharacterPlayableBase::Interact_BoneFire(ABoneFire* bonefire)
{
	if (bonefire->IsLitedboneFire)
	{
		UIManager->OpenBonefireMenu(bonefire->Name);
		GetGameInstance()->GetSubsystem<UManager_Enemy>()->RespawnAll();
		ResetStates();
		BonefireManager->SetLastVisitBonefireName(bonefire->Name);
		InventoryComp->RefillInventory();
		FItemData temp = InventoryComp->GetQuickSlotItem(CurrentQuickSlotNum);
		UIManager->ChangeQuickSlot(temp);

		UIManager->SetPlayerHP(CurrentState.HP, MaxState.HP);
		UIManager->SetPlayerSP(CurrentState.Stemina, MaxState.Stemina);
		UIManager->SetPlayerMP(CurrentState.Mana, MaxState.Mana);
	}
	else
	{
		UIManager->AnnouncementText("Bonefire Lit");
		bonefire->LitBoneFire();
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
			SetCrouchState(false);
			if (!IsMoveable) return;
			FItemData* temp = InventoryComp->GetQuickSlotItemInfo(CurrentQuickSlotNum);
			if (!temp) return;

			FItemBaseData baseData = ItemInfoManager->GetItemBaseData((*temp).ID);
			if ((*temp).Count < 1) return;

			if (baseData.ItemType != EItemType::IT_Refillable &&
				baseData.ItemType != EItemType::IT_Consumable)
			{
				return;
			}

			UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
			if (CurrentMontage && (CurrentMontage->GetName() == "UsePotion_Montage" || CurrentMontage->GetName() == "UsePaper")) return;
			if(!ExcuteItemAffect(baseData.ItemAffectData)) return;
			ReadyToAction();
			InventoryComp->ConsumeItem(temp, 1);
			UpdateCurrentQuickSlotUI();
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
		EqiupMainSlotWeapon((CurrentWeaponSlotNum + 1) % 3);
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
			EqiupQuickSlotItem((CurrentQuickSlotNum + 1) % 3);
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
	CurrentStatus.Soul += soul;
	if (CurrentStatus.Soul < 0) CurrentStatus.Soul = 0;
	UIManager->ChangeSoul(CurrentStatus.Soul);
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
	CurrentQuickSlotNum = slotNum;
	UpdateCurrentQuickSlotUI();
}
void ACharacterPlayableBase::EqiupMainSlotWeapon(int32 slotNum)
{
	if (slotNum > BaseWeaponSlot-1) return;
	CurrentWeaponSlotNum = slotNum; 
	if (CurrentWeapon) CurrentWeapon->SetActiveWeapon(false);

	if (IsValid(MainWeaponSlot[CurrentWeaponSlotNum]))
	{
		MainWeaponSlot[CurrentWeaponSlotNum]->SetActiveWeapon(true);
		CurrentWeapon = MainWeaponSlot[CurrentWeaponSlotNum];
	}
	else
	{
		CurrentWeapon = MainWeaponSlot[3];
		CurrentWeapon->SetActiveWeapon(true);
	}

	if (!CurrentWeapon->bIsTwoHanded)
	{
		AnimInstance->SetIsTwoHandedWeapon(false);
	}
	if (CurrentWeapon->IdleMotion)  AnimInstance->WeaponIdleMotion = CurrentWeapon->IdleMotion;
	if (CurrentWeapon->WalkMotion)	AnimInstance->WeaponWalkMotion = CurrentWeapon->WalkMotion;
	if (CurrentWeapon->RunMotion)	AnimInstance->WeaponRunMotion = CurrentWeapon->RunMotion;

	FItemData temp = InventoryComp->GetWeaponSlotWeapon(CurrentWeaponSlotNum);
	UIManager->ChangeWeaponSlot(temp);
}


void ACharacterPlayableBase::ModifyCurrentHealth(int32 param)
{
	Super::ModifyCurrentHealth(param);
	UIManager->SetPlayerHP(CurrentState.HP, MaxState.HP);
	
}
void ACharacterPlayableBase::ModifyCurrentStemina(int32 stemina )
{
	Super::ModifyCurrentStemina(stemina);
	UIManager->SetPlayerSP(CurrentState.Stemina, MaxState.Stemina);

}
void ACharacterPlayableBase::ModifyCurrentMana(int32 mana)
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
}