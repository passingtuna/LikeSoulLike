#include "PlayerStatusComponent.h"
#include "CharacterPlayableBase.h"

UPlayerStatusComponent::UPlayerStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerStatusComponent::Initialize(ACharacterPlayableBase* InOwner)
{
	OwnerChar = InOwner;
}

void UPlayerStatusComponent::OnHealthChanged()
{
	if (!OwnerChar)
	{
		return;
	}

	OnHPChanged.Broadcast(OwnerChar->GetCurrentState().HP, OwnerChar->GetMaxState().HP);
}

void UPlayerStatusComponent::OnSteminaChanged()
{
	if (!OwnerChar)
	{
		return;
	}

	OnSPChanged.Broadcast(OwnerChar->GetCurrentState().Stemina, OwnerChar->GetMaxState().Stemina);
}

void UPlayerStatusComponent::OnManaChanged()
{
	if (!OwnerChar)
	{
		return;
	}

	if (OwnerChar->GetWorld() && OwnerChar->GetWorld()->GetFirstPlayerController() &&
		OwnerChar->GetWorld()->GetFirstPlayerController()->GetPawn() == OwnerChar)
	{
		OnMPChanged.Broadcast(OwnerChar->GetCurrentState().Mana, OwnerChar->GetMaxState().Mana);
	}
}

void UPlayerStatusComponent::AddSoul(int32 DeltaSoul)
{
	if (!OwnerChar)
	{
		return;
	}

	OwnerChar->CurrentStatus.Soul += DeltaSoul;
	if (OwnerChar->CurrentStatus.Soul < 0)
	{
		OwnerChar->CurrentStatus.Soul = 0;
	}

	OnSoulChanged.Broadcast(OwnerChar->CurrentStatus.Soul);
}

void UPlayerStatusComponent::BroadcastAll()
{
	if (!OwnerChar)
	{
		return;
	}

	OnSoulChanged.Broadcast(OwnerChar->CurrentStatus.Soul);
	OnHPChanged.Broadcast(OwnerChar->GetCurrentState().HP, OwnerChar->GetMaxState().HP);
	OnSPChanged.Broadcast(OwnerChar->GetCurrentState().Stemina, OwnerChar->GetMaxState().Stemina);
	OnMPChanged.Broadcast(OwnerChar->GetCurrentState().Mana, OwnerChar->GetMaxState().Mana);
}

