#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LikeSoulLikeType.h"
#include "EquipmentComponent.generated.h"

class ACharacterPlayableBase;
class AWeaponDefaultBase;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipmentItemChanged, FItemData /*Item*/);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIKESOULLIKE_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEquipmentComponent();

	void Initialize(ACharacterPlayableBase* InOwner);

	// UI 등이 구독하는 이벤트
	FOnEquipmentItemChanged OnWeaponSlotChanged;
	FOnEquipmentItemChanged OnQuickSlotChanged;

	// 슬롯/UI 갱신
	void UpdateCurrentWeaponSlotUI();
	void UpdateCurrentQuickSlotUI();

	// 입력에서 호출
	void WeaponChange();
	void QuickSlotChange();

	// 인벤토리/장비 조작
	void AddWeaponToMainSlot(TSoftClassPtr<AWeaponDefaultBase> WeaponClass, int32 SlotNum);
	void ResetWeaponSlotInfo(int32 SlotNum);
	void EquipQuickSlotItem(int32 SlotNum);
	void EquipMainSlotWeapon(int32 SlotNum);

private:
	UPROPERTY()
	TObjectPtr<ACharacterPlayableBase> OwnerChar;
};

