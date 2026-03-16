#include "LockOnComponent.h"
#include "CharacterPlayableBase.h"
#include "CharacterDefaultBase.h"
#include "SoulLikeController.h"
#include "Camera/CameraComponent.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ULockOnComponent::ULockOnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULockOnComponent::Initialize(ACharacterPlayableBase* InOwner, UCameraComponent* InCamera, ASoulLikeController* InController, UCharacterMovementComponent* InMovement)
{
	OwnerChar = InOwner;
	CameraComp = InCamera;
	PlayerController = InController;
	CharacterMovement = InMovement;
}

void ULockOnComponent::TickLockOn(float DeltaTime)
{
	if (OwnerChar)
	{
		bIsLockOn = OwnerChar->GetIsLockOnState();
		if (!LockOnTargetChar)
		{
			LockOnTargetChar = OwnerChar->GetLockOnTarget();
		}
	}

	if (!bIsLockOn || !LockOnTargetChar)
	{
		return;
	}

	if (CheckContinueLockOn())
	{
		UpdateLockOnRotation(DeltaTime);
	}
	else
	{
		ReleaseLockOn();
	}
}

void ULockOnComponent::ToggleLockOn()
{
	if (bIsLockOn)
	{
		ReleaseLockOn();
	}
	else
	{
		TryLockOn();
	}
}

void ULockOnComponent::TryLockOn()
{
	if (!OwnerChar || !CameraComp || !PlayerController || !GetWorld())
	{
		return;
	}

	TArray<FOverlapResult> Overlaps;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(SearchRadius);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerChar);

	GetWorld()->OverlapMultiByChannel(
		Overlaps,
		OwnerChar->GetActorLocation(),
		FQuat::Identity,
		ECC_Pawn,
		Sphere,
		Params
	);

	ACharacterDefaultBase* BestTarget = nullptr;
	float ClosestDistSq = MaxLockOnDistance * MaxLockOnDistance;

	for (auto& Target : Overlaps)
	{
		ACharacterDefaultBase* CharacterBase = Cast<ACharacterDefaultBase>(Target.GetActor());
		if (!CharacterBase || CharacterBase->GetIsDead())
		{
			continue;
		}

		if (CheckTargetLock(CharacterBase, ClosestDistSq))
		{
			BestTarget = CharacterBase;
		}
	}

	if (!BestTarget)
	{
		return;
	}

	bIsLockOn = true;
	LockOnTargetChar = BestTarget;
	OwnerChar->SetLockOnState(true, BestTarget);
	LockOnTargetChar->StartLockOnUI();

	OwnerChar->bUseControllerRotationYaw = true;
	if (CharacterMovement)
	{
		CharacterMovement->bOrientRotationToMovement = false;
	}
}

bool ULockOnComponent::CheckTargetLock(ACharacterDefaultBase* Target, float& ClosestDistSq) const
{
	if (!OwnerChar || !CameraComp || !PlayerController || !Target)
	{
		return false;
	}

	if (Target == OwnerChar)
	{
		return false;
	}

	// 중립과 아군은 록온 타겟에서 제외
	if (!OwnerChar->CheckHostileChar(Target, false))
	{
		return false;
	}

	if (Target->GetIsDead())
	{
		return false;
	}

	const FVector PlayerLoc = OwnerChar->GetActorLocation();
	const FVector CameraLoc = CameraComp->GetComponentLocation();
	const FVector TargetLoc = Target->GetActorLocation();

	// 거리 체크: 지금까지 가장 가까웠던 거리와 비교
	const float DistSq = FVector::DistSquared(PlayerLoc, TargetLoc);
	if (DistSq > ClosestDistSq)
	{
		return false;
	}

	// 시야각 1차 체크
	const FVector ToTarget = (TargetLoc - CameraLoc).GetSafeNormal();
	const float Dot = FVector::DotProduct(CameraComp->GetForwardVector(), ToTarget);
	if (Dot < MinViewDot)
	{
		return false;
	}

	// 화면 투영되어있는지 체크
	FVector2D ScreenPos;
	const bool bOnScreen = UGameplayStatics::ProjectWorldToScreen(PlayerController, TargetLoc, ScreenPos);
	if (!bOnScreen)
	{
		return false;
	}

	// 화면 해상도 체크
	int32 SizeX = 0, SizeY = 0;
	PlayerController->GetViewportSize(SizeX, SizeY);
	if (ScreenPos.X < 0 || ScreenPos.X > SizeX || ScreenPos.Y < 0 || ScreenPos.Y > SizeY)
	{
		return false;
	}

	// 시야 가림 체크
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerChar);
	Params.AddIgnoredActor(OwnerChar->GetOwner());

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		CameraLoc,
		TargetLoc,
		ECC_Visibility,
		Params
	);

	if (bHit && Hit.GetActor() != Target)
	{
		return false;
	}

	ClosestDistSq = DistSq;
	return true;
}

bool ULockOnComponent::CheckContinueLockOn() const
{
	if (!OwnerChar || !PlayerController || !LockOnTargetChar)
	{
		return false;
	}

	if (LockOnTargetChar->GetIsDead())
	{
		return false;
	}

	const FVector PlayerLoc = OwnerChar->GetActorLocation();
	const FVector TargetLoc = LockOnTargetChar->GetActorLocation();
	const float DistSq = FVector::DistSquared(PlayerLoc, TargetLoc);

	if (DistSq > (MaxLockOnDistance * MaxLockOnDistance))
	{
		return false;
	}

	FVector2D ScreenPos;
	const bool bOnScreen = UGameplayStatics::ProjectWorldToScreen(PlayerController, TargetLoc, ScreenPos);
	if (!bOnScreen)
	{
		return false;
	}

	return true;
}

void ULockOnComponent::ReleaseLockOn()
{
	if (!OwnerChar)
	{
		return;
	}

	bIsLockOn = false;
	OwnerChar->SetLockOnState(false, nullptr);

	if (LockOnTargetChar)
	{
		LockOnTargetChar->EndLockOnUI();
	}

	if (CharacterMovement)
	{
		CharacterMovement->bOrientRotationToMovement = true;
	}

	OwnerChar->bUseControllerRotationYaw = false;
	LockOnTargetChar = nullptr;
}

void ULockOnComponent::UpdateLockOnRotation(float DeltaTime) const
{
	if (!OwnerChar || !CameraComp || !LockOnTargetChar)
	{
		return;
	}

	const FVector CameraLoc = CameraComp->GetComponentLocation();
	FVector TargetLoc = LockOnTargetChar->GetMesh()->GetSocketLocation(TEXT("Death_Torso"));
	TargetLoc.Z -= 60.0f;

	const FRotator DesiredRot = UKismetMathLibrary::FindLookAtRotation(CameraLoc, TargetLoc);

	if (AController* Controller = OwnerChar->GetController())
	{
		const FRotator CurrentRot = Controller->GetControlRotation();
		const FRotator NewRot = FMath::RInterpTo(CurrentRot, DesiredRot, DeltaTime, RotationInterpSpeed);
		Controller->SetControlRotation(NewRot);
	}
}

