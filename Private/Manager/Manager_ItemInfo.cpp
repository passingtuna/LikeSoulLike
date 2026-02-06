// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager_ItemInfo.h"
#include "Engine/AssetManager.h"
#include "PDA_ItemDetailInfo.h"
#include "DA_ItemInstantData.h"
#include "DA_ItemInfoManager.h"
#include "WeaponDefaultBase.h"


void UManager_ItemInfo::Initialize(FSubsystemCollectionBase& Collection)
{
    AssetManager = &UAssetManager::Get();
    static const FString AssetPath = "/Game/DataAsset/Manager/ItemInfoManagerDataAsset.ItemInfoManagerDataAsset";
    UDA_ItemInfoManager * ManagerAsset = Cast<UDA_ItemInfoManager>(StaticLoadObject(UDA_ItemInfoManager::StaticClass(), nullptr, *AssetPath));
    if (ManagerAsset)
    {
        arrIconAtlas = ManagerAsset->arrIconAtlas;
    }
    
    TArray<FPrimaryAssetId> ItemIds;
    AssetManager->GetPrimaryAssetIdList(TEXT("Item"), ItemIds);

    TSharedPtr<FStreamableHandle> Handle = AssetManager->LoadPrimaryAssets(ItemIds);
    Handle->WaitUntilComplete();
    for (const FPrimaryAssetId& Id : ItemIds)
    {
        if (UPDA_ItemDetailInfo* ItemDA = Cast<UPDA_ItemDetailInfo>(AssetManager->GetPrimaryAssetObject(Id)))
        {
            mapItemBaseData.Add(Id.PrimaryAssetName, ItemDA->ItemBaseData);
        }
    }
}

void UManager_ItemInfo::LoadNoneSyncItemInfo(FName Id)
{
    FPrimaryAssetId AssetId("Item", Id);
    FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject( this, &UManager_ItemInfo::OnItemDetailLoaded, AssetId);
    AssetManager->LoadPrimaryAsset(AssetId, TArray<FName>(), Delegate , 0);
}
void UManager_ItemInfo::OnItemDetailLoaded(FPrimaryAssetId AssetId) //비동기 콜백 아직은 노쓸모
{
    if (UObject* Obj = UAssetManager::Get().GetPrimaryAssetObject(AssetId))
    {
        if (UPDA_ItemDetailInfo* ItemInfo = Cast<UPDA_ItemDetailInfo>(Obj))
        { 
        }
    }
}
FString UManager_ItemInfo::GetItemNameString(FItemData& ItemData)
{
    FString result = "";
    EWeaponInfusionType type = ItemData.Infusion;
    switch (type)
    {
        case EWeaponInfusionType::EWIT_Heavy: result = INFUSION_HEAVY_TEXT; break;
        case EWeaponInfusionType::EWIT_Dex: result = INFUSION_DEXTURY_TEXT; break;
        case EWeaponInfusionType::EWIT_Devine: result = INFUSION_DEVINE_TEXT; break;
        case EWeaponInfusionType::EWIT_Magic: result = INFUSION_MAGIC_TEXT; break;
    }
    FItemBaseData temp = GetItemBaseData(ItemData.ID);
    result += temp.Name.ToString();

    if (ItemData.Upgrade > 0)
    {
        result += " (+ ";
        result += FString::FromInt(ItemData.Upgrade);
        result += ")";
    }

    if (temp.MaxStackCount > 1)
    {
        result += " X ";
        result += FString::FromInt(ItemData.Count);
    }

    return result;
}

FItemPresentationData UManager_ItemInfo::GetItemPresentationData(FName Id, bool NeedSyncLoad)
{
    FItemPresentationData result;
    FPrimaryAssetId AssetId("Item", Id);
    UPDA_ItemDetailInfo* DataAsset;

    if (UObject* AssetObj = AssetManager->GetPrimaryAssetObject(AssetId))
    {
        DataAsset = Cast< UPDA_ItemDetailInfo>(AssetObj);
        result = DataAsset->ItemDetailData;
        TSubclassOf<AWeaponDefaultBase> SpwanBp = result.Bp_Weapon.Get(); // 로드 확인

        if (!SpwanBp)
        {
            if (NeedSyncLoad)
            {
                SpwanBp = result.Bp_Weapon.LoadSynchronous(); //동기 로드 시도
                UE_LOG(LogTemp, Log, TEXT("동기 로드 시도 : %s"), *result.Bp_Weapon.Get()->GetName());
            }
            else
            {
                FSoftObjectPath Path = result.Bp_Weapon.ToSoftObjectPath();

                AssetManager->GetStreamableManager().RequestAsyncLoad(Path, FStreamableDelegate::CreateLambda([result]()
                    {
                        UClass* LoadedClass = result.Bp_Weapon.Get();
                        if (LoadedClass)
                        {
                            UE_LOG(LogTemp, Log, TEXT("Weapon 클래스 로드 완료: %s"), *LoadedClass->GetName());
                        }
                    }));
            }
        }
    }
    return result;

    /*
    //현재 구조로 인해 프라이머리 데이터 에셋은 파일단위 캐싱에만 사용 비동기 로드는 SoftPtr로

    if (UObject* AssetObj = AssetManager->GetPrimaryAssetObject(AssetId))
    {
         DataAsset = Cast< UPDA_ItemDetailInfo>(AssetObj);
        result = DataAsset->ItemDetailData;
    }
    else //데이터를 써야되는데 비동기로드를 안해놨으면 동기로드해서 리턴해준다
    {
        TSharedPtr<FStreamableHandle> Handle = AssetManager->LoadPrimaryAsset(AssetId);
        if (Handle.IsValid())
        {
            UObject* Obj = Handle->GetLoadedAsset();
            if (!Obj)
            {
                Handle->WaitUntilComplete();
                Obj = Handle->GetLoadedAsset();
            }
            DataAsset = Cast<UPDA_ItemDetailInfo>(Obj);
            result = DataAsset->ItemDetailData;
        }
    }
    */
}
FItemBaseData UManager_ItemInfo::GetItemBaseData(FName Id)
{
    FItemBaseData result;
    FItemBaseData* temp = mapItemBaseData.Find(Id);
    if (temp) result = *temp;
    return result;
}

FIconData UManager_ItemInfo::GetIconByID(FName ID)
{
    FIconData result;
    int32 inputIndex = GetItemBaseData(ID).IconIndex;
    if (ID == NAME_None) inputIndex = 1;
    const int32 atlasIndex = inputIndex / ICON_ATLAS_NUM;

    if (!arrIconAtlas.IsValidIndex(atlasIndex))
    {
        return result;
    }
    FIconAtlasData& data = arrIconAtlas[atlasIndex];

    if (!data.Atlas.IsNull())
    {
        data.LoadedAtlas = data.Atlas.LoadSynchronous();
    }
    else
    {
        return result;
    }

    UTexture2D* texture = data.LoadedAtlas;

    const int32 localIndex = inputIndex % ICON_ATLAS_NUM;
    const int32 atlasUvX = (localIndex / ICON_MAX_ROW) * ICON_SIZE;
    const int32 atlasUvY = (localIndex % ICON_MAX_ROW) * ICON_SIZE;

    int32 atlasWidth = texture->GetSizeX();
    int32 atlasHeight = texture->GetSizeY();
    int32 iconX = (localIndex % ICON_MAX_ROW) * ICON_SIZE;
    int32 iconY = (localIndex / ICON_MAX_ROW) * ICON_SIZE;

    result.Uv = FVector2D(
        static_cast<float>(iconX) / atlasWidth,
        static_cast<float>(iconY) / atlasHeight
    );

    result.IconSizeNormalized = FVector2D(
        static_cast<float>(ICON_SIZE) / atlasWidth,
        static_cast<float>(ICON_SIZE) / atlasHeight
    );

    result.Atlas = texture;

    return result;
}