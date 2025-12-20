// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager_Projectile.h"
#include "DA_ProjectileManagerData.h"
#include "ProJectile_Default.h"



void UManager_Projectile::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	static const FString AssetPath = "/Game/DataAsset/Manager/ProjectileManagerDataAsset.ProjectileManagerDataAsset";
	ProjectileDataAsset = Cast<UDA_ProjectileManagerData>(StaticLoadObject(UDA_ProjectileManagerData::StaticClass(), nullptr, *AssetPath));

	if (ProjectileDataAsset)
	{
		mapResistProjectile = (ProjectileDataAsset->mapResistProjectile);
		for (auto & Elem : ProjectileDataAsset->mapResistProjectile)
		{
			mapProjectilePool.Add(Elem.Key);
		}

	}
}


AProjectile_Default* UManager_Projectile::GetProjectile(FName ProJectileNmae)
{
	TSubclassOf < AProjectile_Default > * ProjectileType = mapResistProjectile.Find(ProJectileNmae);
	TArray<AProjectile_Default*> *Pool = mapProjectilePool.Find(ProJectileNmae);
	UWorld* World = GetWorld();
	if (!World) return nullptr;
	if(!Pool) return nullptr;
	if (!ProjectileType) return nullptr;
	for (auto& Elem : (*Pool))
	{
		if (Elem->GetIsReadyToFire())
		{
			if (!IsValid(Elem)) continue;
			Elem->ReadyProjectile();
			return Elem;
		}
	}

	AProjectile_Default* NewProjectile = GetWorld()->SpawnActor<AProjectile_Default>(*ProjectileType);

	NewProjectile->ReadyProjectile();
	Pool->Add(NewProjectile);
	return NewProjectile;
}

void UManager_Projectile::CheckProjectile()
{
}
