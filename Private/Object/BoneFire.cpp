// Fill out your copyright notice in the Description page of Project Settings.


#include "BoneFire.h"
#include "Manager_Bonefire.h"
#include "Components/HeterogeneousVolumeComponent.h"
// Sets default values
ABoneFire::ABoneFire()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABoneFire::BeginPlay()
{
	Super::BeginPlay();

    TArray<UHeterogeneousVolumeComponent*> Components;
    GetComponents<UHeterogeneousVolumeComponent>(Components);

    for (UHeterogeneousVolumeComponent* Comp : Components)
    {
        if (Comp && Comp->GetName().StartsWith(TEXT("FireScene")))
        {
            Comp->SetVisibility(false);
            FirePoint.Add(Comp);
        }
    }


    TArray<USceneComponent*> SceneComponents;
    GetComponents<USceneComponent>(SceneComponents);

    for (USceneComponent* Comp : SceneComponents)
    {
        if (Comp && Comp->GetName().StartsWith(TEXT("WarpPos")))
        {
            WarpPosition = Comp;
        }
    }

     IsLitedboneFire = false;

    GetWorld()->GetGameInstance()->GetSubsystem<UManager_Bonefire>()->AddBonefire(this);
    
}

// Called every frame
void ABoneFire::Tick(float DeltaTime)
{
	 //Super::Tick(DeltaTime);
}


FTransform ABoneFire::GetRespawnTransform()
{
    return WarpPosition ? WarpPosition->GetComponentTransform() : GetActorTransform();
}

void ABoneFire::LitBoneFire()
{
    GetWorld()->GetGameInstance()->GetSubsystem<UManager_Bonefire>()->AddLitedBonefire(Name);
    IsLitedboneFire = true;
    for (auto& Elem : FirePoint)
    {
        Elem->SetVisibility(true);
    }
    GetWorld()->GetGameInstance()->GetSubsystem<UManager_Bonefire>()->SetLastVisitBonefireName(Name);
}