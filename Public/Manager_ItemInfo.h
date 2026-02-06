// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LikeSoulLikeType.h"
#include "Manager_ItemInfo.generated.h"

#define ICON_SIZE 500
#define ICON_MAX_ROW 6
#define ICON_MAX_LINE 6
#define ICON_ATLAS_NUM 36
#define INFUSION_HEAVY_TEXT   TEXT("중후한 ")
#define INFUSION_DEXTURY_TEXT TEXT("예리한 ")
#define INFUSION_DEVINE_TEXT  TEXT("신성한 ")
#define INFUSION_MAGIC_TEXT   TEXT("마법의 ")


/**
 * 
 */
class UAssetManager;
class UDA_ItemInfoManager;
UCLASS()
class LIKESOULLIKE_API UManager_ItemInfo : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<FIconAtlasData> arrIconAtlas;
	TMap<FName, FItemBaseData> mapItemBaseData;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UAssetManager* AssetManager;
	void Init();
	void LoadNoneSyncItemInfo(FName Id);
	FString GetItemNameString(FItemData & ItemData);
	FItemPresentationData GetItemPresentationData(FName Id , bool NeedSyncLoad);
	FItemBaseData GetItemBaseData(FName Id);
	FIconData GetIconByID(FName ID);
	void OnItemDetailLoaded(FPrimaryAssetId AssetId);
};
