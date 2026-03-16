#include "InteractComponent.h"
#include "CharacterPlayableBase.h"
#include "DropItem.h"
#include "BoneFire.h"
#include "InventoryComponent.h"
#include "Manager_ItemInfo.h"
#include "Manager_Enemy.h"
#include "Manager_Bonefire.h"
#include "EquipmentComponent.h"
#include "PlayerStatusComponent.h"

#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"

UInteractComponent::UInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractComponent::Initialize(ACharacterPlayableBase* InOwner)
{
	OwnerChar = InOwner;
}

void UInteractComponent::HandleInteract()
{
	if (!OwnerChar || !OwnerChar->GetIsMoveable())
	{
		return;
	}

	SearchAndExecute();
}

void UInteractComponent::SearchAndExecute()
{
	if (!OwnerChar)
	{
		return;
	}

	TArray<FOverlapResult> Overlaps;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(SearchRadius);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerChar);

	FVector SearchLoc = OwnerChar->GetActorLocation() + (OwnerChar->GetActorForwardVector() * ForwardOffset);
	SearchLoc.Z += ZOffset;

	if (!GetWorld())
	{
		return;
	}

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
		if (ADropItem* DropItem = Cast<ADropItem>(Target.GetActor()))
		{
			HandleDropItem(DropItem);
			// 아이템과 상호작용하면 다른 대상은 무시
			return;
		}

		if (ABoneFire* Bonefire = Cast<ABoneFire>(Target.GetActor()))
		{
			HandleBoneFire(Bonefire);
			return;
		}
	}
}

void UInteractComponent::HandleDropItem(ADropItem* DropItem)
{
	if (!OwnerChar || !DropItem)
	{
		return;
	}

	OwnerChar->PlaySound(TEXT("GetItem"));

	UInventoryComponent* InventoryComp = OwnerChar->GetInventoryComponent();
	UManager_ItemInfo* ItemInfoManager = OwnerChar->GetItemInfoManager();

	if (!InventoryComp || !ItemInfoManager)
	{
		return;
	}

	if (DropItem->IsSoul)
	{
		OwnerChar->ModifyCurrentSoul(DropItem->Soul);
		DropItem->Destroy();
		return;
	}

	FString TempString;
	FItemData TempItemCount = DropItem->ItemInfo;
	const int32 Result = InventoryComp->PutInItemToInventory(DropItem->ItemInfo);
	if (Result <= 0)
	{
		DropItem->Destroy();
	}
	else // 인벤 창고 꽉차서 필드에 남김
	{
		TempString = TEXT("가득참 : ");
		DropItem->ItemInfo.Count = Result;
		TempItemCount.Count -= Result;
	}
	TempString += ItemInfoManager->GetItemNameString(TempItemCount);

	OnShowGetItemUI.Broadcast(DropItem->ItemInfo, FText::FromString(TempString));
	if (UEquipmentComponent* EquipComp = OwnerChar->GetEquipmentComponent())
	{
		EquipComp->UpdateCurrentQuickSlotUI();
	}
}

void UInteractComponent::HandleBoneFire(ABoneFire* Bonefire)
{
	if (!OwnerChar || !Bonefire)
	{
		return;
	}

	UManager_Enemy* EnemyManager = OwnerChar->GetEnemyManager();
	UManager_Bonefire* BonefireManager = OwnerChar->GetBonefireManager();
	UInventoryComponent* InventoryComp = OwnerChar->GetInventoryComponent();

	if (!EnemyManager || !BonefireManager || !InventoryComp)
	{
		return;
	}

	if (Bonefire->IsLitedboneFire)
	{
		OnOpenBonefireMenu.Broadcast(Bonefire->Name);
		EnemyManager->RespawnAll();
		OwnerChar->ResetStates();
		BonefireManager->SetLastVisitBonefireName(Bonefire->Name);
		InventoryComp->RefillInventory();
		if (UEquipmentComponent* EquipComp = OwnerChar->GetEquipmentComponent())
		{
			EquipComp->UpdateCurrentQuickSlotUI();
		}
		if (UPlayerStatusComponent* StatusComp = OwnerChar->GetPlayerStatusComponent())
		{
			StatusComp->BroadcastAll();
		}
	}
	else
	{
		OnAnnouncementText.Broadcast(TEXT("Bonefire Lit"));
		Bonefire->LitBoneFire();
	}
}

