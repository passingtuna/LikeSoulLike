#include "DeathRespawnComponent.h"
#include "CharacterPlayableBase.h"
#include "AnimInstanceDefaultBase.h"
#include "DA_DefaultMotion.h"
#include "InventoryComponent.h"
#include "Manager_Bonefire.h"
#include "Manager_Enemy.h"
#include "EquipmentComponent.h"
#include "PlayerStatusComponent.h"

UDeathRespawnComponent::UDeathRespawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDeathRespawnComponent::Initialize(ACharacterPlayableBase* InOwner)
{
	OwnerChar = InOwner;
}

void UDeathRespawnComponent::HandleDeath()
{
	if (!OwnerChar || !OwnerChar->GetWorld())
	{
		return;
	}

	OwnerChar->SetDeadStateInternal(true);
	if (OwnerChar->GetAnimInstance())
	{
		OwnerChar->GetAnimInstance()->StopAllMontages(0);
	}

	if (OwnerChar->DefaultMotion && OwnerChar->DefaultMotion->DiedMotion)
	{
		OwnerChar->PlayAnimMontage(OwnerChar->DefaultMotion->DiedMotion, 1);
	}

	OnAnnouncementFadeOut.Broadcast(TEXT("You Died"));

	FTimerHandle Handle;
	OwnerChar->GetWorld()->GetTimerManager().SetTimer(
		Handle,
		[this]()
		{
			HandleRespawnAfterDelay();
		},
		3.0f,
		false
	);
}

void UDeathRespawnComponent::HandleRespawnAfterDelay()
{
	if (!OwnerChar)
	{
		return;
	}

	if (UManager_Enemy* EnemyManager = OwnerChar->GetEnemyManager())
	{
		EnemyManager->RespawnAll();
	}

	OwnerChar->DropSoul();

	if (UManager_Bonefire* BonefireManager = OwnerChar->GetBonefireManager())
	{
		BonefireManager->RespawnLastBonefire(OwnerChar);
	}

	OwnerChar->ResetStates();

	if (OwnerChar->InventoryComp)
	{
		OwnerChar->InventoryComp->RefillInventory();
	}

	if (UPlayerStatusComponent* StatusComp = OwnerChar->GetPlayerStatusComponent())
	{
		StatusComp->BroadcastAll();
	}

	if (UEquipmentComponent* EquipComp = OwnerChar->GetEquipmentComponent())
	{
		EquipComp->UpdateCurrentQuickSlotUI();
	}

	OnBossHPUI.Broadcast(false);

	FTimerHandle Handle2;
	OwnerChar->GetWorld()->GetTimerManager().SetTimer(
		Handle2,
		[this]()
		{
			OnRequestInvisibleAnnouncement.Broadcast();
		},
		1.0f,
		false
	);
}

