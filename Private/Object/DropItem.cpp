// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItem.h"

// Sets default values
ADropItem::ADropItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	IsSoul = false;
	Soul = 0;
}

// Called when the game starts or when spawned
void ADropItem::BeginPlay()
{
	Super::BeginPlay();
	InitItem();
}

// Called every frame
void ADropItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ADropItem::InitItem()
{
	TArray<UNiagaraComponent*> Components;
	GetComponents<UNiagaraComponent>(Components);
	for (UNiagaraComponent* Comp : Components)
	{
		if (Comp)
		{
			if (IsSoul)
			{
				if (Comp->GetName() == "SoulComp")
				{
					DropSoul = Comp;
					DropSoul->Activate();
				}
				else
				{
					DropItem = Comp;
					DropItem->Deactivate();
				}
			}
			else
			{
				if (Comp->GetName() == "ItemComp")
				{
					DropItem = Comp;
					DropItem->Activate();
				}
				else
				{
					DropSoul = Comp;
					DropSoul->Deactivate();
				}
			}
		}
	}
}


void ADropItem::SetDropSoul(int32 nSoul)
{
	IsSoul = true;
	InitItem();
	Soul = nSoul;
}