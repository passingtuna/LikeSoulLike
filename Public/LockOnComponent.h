#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockOnComponent.generated.h"

class ACharacterDefaultBase;
class ACharacterPlayableBase;
class UCameraComponent;
class ASoulLikeController;
class UCharacterMovementComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIKESOULLIKE_API ULockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULockOnComponent();

	void Initialize(ACharacterPlayableBase* InOwner, UCameraComponent* InCamera, ASoulLikeController* InController, UCharacterMovementComponent* InMovement);

	void TickLockOn(float DeltaTime);
	void ToggleLockOn();
	void TryLockOn();
	void ReleaseLockOn();

	bool IsLockedOn() const { return bIsLockOn; }
	ACharacterDefaultBase* GetTarget() const { return LockOnTargetChar.Get(); }

private:
	bool CheckTargetLock(ACharacterDefaultBase* Target, float& ClosestDistSq) const;
	bool CheckContinueLockOn() const;
	void UpdateLockOnRotation(float DeltaTime) const;

private:
	UPROPERTY()
	TObjectPtr<ACharacterPlayableBase> OwnerChar;

	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY()
	TObjectPtr<ASoulLikeController> PlayerController;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> CharacterMovement;

	UPROPERTY()
	TObjectPtr<ACharacterDefaultBase> LockOnTargetChar;

	UPROPERTY()
	bool bIsLockOn = false;

	UPROPERTY(EditAnywhere, Category = "LockOn")
	float SearchRadius = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "LockOn")
	float MaxLockOnDistance = 3000.0f;

	UPROPERTY(EditAnywhere, Category = "LockOn")
	float MinViewDot = 0.6f;

	UPROPERTY(EditAnywhere, Category = "LockOn")
	float RotationInterpSpeed = 20.0f;
};

