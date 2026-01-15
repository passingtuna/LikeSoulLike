#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "LikeSoulLikeType.generated.h"


class UDA_ActionData;
class ACharacterDefaultBase;
class AWeaponDefaultBase;
UENUM(BlueprintType)
enum class EItemType : uint8
{
    IT_Consumable   UMETA(DisplayName = "Consumable"),
    IT_Armor        UMETA(DisplayName = "Armor"),
    IT_Weapon       UMETA(DisplayName = "Weapon"),
    IT_KeyItem      UMETA(DisplayName = "KeyItem"),
    IT_Refillable   UMETA(DisplayName = "Refillable"),
    IT_Material      UMETA(DisplayName = "Material"),
};

UENUM(BlueprintType)
enum EStatusConditionType
{
    SCT_Poison, SCT_Buff, SCT_Debuff, SCT_Stun
};
UENUM(BlueprintType)
enum class EAffectionType : uint8
{
    AT_MaxHP, 
    AT_CurrentHp, 
    AT_MaxStemina, 
    AT_CurrentStemina, 
    AT_MaxMana,
    AT_CurrentMana,
    AT_FireResist,
    AT_LightningResist, 
    AT_DevineResist, 
    AT_PhysicsDamage, 
    AT_FireDamage, 
    AT_LightningDamage ,
    AT_MagicDamage , 
    AT_DevineDamage
};
UENUM(BlueprintType)
enum class EItemObjectBelongUI : uint8
{
    IOBU_Inventory,
    IOBU_Equipment, 
    IOBU_QuickSlot,
    IOBU_Storage
};

UENUM(BlueprintType)
enum EItemEnchantType
{
    IET_Strong, IET_Dex, IET_StrongDex, IET_Intellegence , IET_Devine
};

UENUM(BlueprintType)
enum class EItemFilter : uint8
{
    EIF_None,
    EIF_ALL,
    EIF_Equipment,
    EIF_Consumable
};

UENUM(BlueprintType)
enum class EItemButtonActionType : uint8
{
    EBAT_QucikSlot,
    EBAT_WeaponSlot,
    EBAT_EquipmentSlot,
    EBAT_InventorySlot,
    EBAT_StorageSlot
};

UENUM(BlueprintType)
enum class ETeam : uint8
{
    T_Player,T_Neutral, T_Monster
};


UENUM(BlueprintType)
enum class EWeaponInfusionType : uint8
{
    EWIT_None, EWIT_Heavy, EWIT_Dex, EWIT_Devine, EWIT_Magic
};



UENUM(BlueprintType)
enum class EAttackDecisionType : uint8
{
    ADT_Sweep               UMETA(DisplayName = "Sweep"),
    ADT_AreaOnce            UMETA(DisplayName = "AreaOnce"),
    ADT_AreaTick            UMETA(DisplayName = "AreaTick"),
};

UENUM(BlueprintType)
enum class EDecisionShapeType : uint8
{
    AST_Sphere         UMETA(DisplayName = "Sphere"),
    AST_Capsule        UMETA(DisplayName = "Capsule"),
    AST_Box            UMETA(DisplayName = "Box"),
};

UENUM(BlueprintType)
enum class EActionInputType : uint8
{
    AIT_WeakAttack          UMETA(DisplayName = "WeakAttack"),
    AIT_StrongAttack        UMETA(DisplayName = "StrongAttack"),
    AIT_WeaponAction        UMETA(DisplayName = "WeaponAction"),
    AIT_WeaponSkill         UMETA(DisplayName = "WeaponSkill"),
    AIT_Avoid               UMETA(DisplayName = "Avoid")
};

UENUM(BlueprintType)
enum class EActionType : uint8
{
    AT_MoveDefault  UMETA(DisplayName = "Move"),    //이동 기본 시퀀스 생성
    AT_Instance     UMETA(DisplayName = "Instance"),//Started 실행 > 종료, Triggered,Completed 무시
    AT_Release      UMETA(DisplayName = "Release"),   //Completed 시점에 캐릭터 이동 실행
    AT_Avoid        UMETA(DisplayName = "Avoid"),   //Completed 시점에 캐릭터 이동 실행
    AT_Charge       UMETA(DisplayName = "Charge"),  //Started > 자동으로 루프 ,Completed시 실행 캐릭터 이동 불가
    AT_Hold         UMETA(DisplayName = "Hold"),    //Started > 자동으로 루프 ,Completed시 실행 캐릭터 이동 가능
};

UENUM(BlueprintType)
enum class EDecisionType : uint8
{
    DT_Attack       UMETA(DisplayName = "Attack"),
    DT_Guard        UMETA(DisplayName = "Guard"),
    DT_Parry        UMETA(DisplayName = "Parry"),
    DT_Bounce        UMETA(DisplayName = "Bounce"),
    DT_Avoid         UMETA(DisplayName = "Avoid")
};

UENUM(BlueprintType)
enum class EProjectileLaunchType : uint8
{
    PLT_Single      UMETA(DisplayName = "Single"),
    PLT_Fan         UMETA(DisplayName = "Fan"),
    PLT_Multiple    UMETA(DisplayName = "Mutiple"),
    PLT_Burst       UMETA(DisplayName = "Burst")
};


USTRUCT(BlueprintType)
struct FComboActionData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EActionInputType InputType = EActionInputType::AIT_WeakAttack;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UDA_ActionData* ComboAction = nullptr;
};

struct FPlayerStatus
{
    int32 Level           = 15;
    int32 Health          = 10;
    int32 Endurance       = 10;
    int32 Mentality       = 10;
    int32 Strong          = 10;
    int32 Dextery         = 10;
    int32 Intellegence    = 10;
    int32 Faith           = 10;
    int32 Soul            = 0;
};

USTRUCT(BlueprintType)
struct FCharacterState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 HP = 100;
    int32 Stemina = 100;
    int32 Mana = 100;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Toughness = 10;
    int32 Weight = 100;
};

struct FResistData
{
    int32 PhysicalResist  = 0;
    int32 FireResist      = 0;
    int32 LightningResist = 0;
    int32 MagicResist     = 0;
    int32 DevineResist    = 0;
};
USTRUCT(BlueprintType)
struct FDamageData
{
    GENERATED_BODY()
    FDamageData operator*(float Scale) const
    {
        FDamageData Result = *this;
        Result.PhysicalDamage *= Scale;
        Result.DevineDamage *= Scale;
        Result.FireDamage *= Scale;
        Result.MagicDamage *= Scale;
        Result.LightningDamage *= Scale;
        Result.StaggerDamage *= Scale;
        return Result;
    }
    FDamageData operator+(FDamageData damage) const
    {
        FDamageData Result = *this;
        Result.PhysicalDamage += damage.PhysicalDamage;
        Result.DevineDamage += damage.DevineDamage;
        Result.FireDamage += damage.FireDamage;
        Result.MagicDamage += damage.MagicDamage;
        Result.LightningDamage += damage.LightningDamage;
        Result.StaggerDamage += damage.LightningDamage;
        return Result;
    }

    FDamageData operator=(float damage) const
    {
        FDamageData Result = *this;
        Result.PhysicalDamage = damage;
        Result.DevineDamage = damage;
        Result.FireDamage = damage;
        Result.MagicDamage = damage;
        Result.LightningDamage = damage;
        Result.StaggerDamage = damage;
        Result.IsParryable = (bool)damage;
        Result.IsAvoidable = (bool)damage;
        Result.IsGuardable = (bool)damage;
        return Result;
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PhysicalDamage = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FireDamage = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LightningDamage = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MagicDamage = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DevineDamage = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StaggerDamage = 0;
    bool IsParryable = 0;
    bool IsAvoidable = 0;
    bool IsGuardable = 0;
};


struct FStatusCondition
{
    FName ConditionName;
    EStatusConditionType ConditionType;
    float DurationTime;
    EAffectionType AffectionType;
    float AffectionNumber;
    bool bIsPermanent;
    bool isMultiplyer;
};


USTRUCT(BlueprintType)
struct FWeaponScaleData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BaseScale     = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StrScale      = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DexScale      = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FaiScale   = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float IntScale    = 1;
};

USTRUCT(BlueprintType)
struct FWeaponSpawnEditData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MeshName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName SocketName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTransform Transform;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool IsVisible = true;
};


struct FDamageCalculateData
{
    int32 BaseDamage;
    float MotionMultipler;
};


struct FWeaponMeshInitData
{
    UPrimitiveComponent* Mesh;
    FName SocketName;
    FTransform InitTransForm;
    bool IsVisibility;
};

USTRUCT(BlueprintType)
struct FWeaponDamageData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDamageData BaseDamage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FWeaponScaleData BaseScale;
};

USTRUCT(BlueprintType)
struct FItemData
{
    GENERATED_BODY()
    bool IsEquiped = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ID = NAME_None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Count = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Upgrade = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EWeaponInfusionType Infusion = EWeaponInfusionType::EWIT_None;
};

USTRUCT(BlueprintType)
struct FItemAffectData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EAffectionType AffectType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Value;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;
};
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EItemMoveRestriction : uint8
{
    None = 0 UMETA(Hidden),
    NoStorage = 1 << 0,
    NoDrop = 1 << 1,
    NoSell = 1 << 2,
};
ENUM_CLASS_FLAGS(EItemMoveRestriction)

USTRUCT(BlueprintType)
struct FItemBaseData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 IconIndex;
    UPROPERTY(EditDefaultsOnly, meta = (Bitmask, BitmaskEnum = "/Script/LikeSoulLike.EItemMoveRestriction"))
    int32  MoveRestriction = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemType ItemType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxStackCount;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "ItemType == EItemType::IT_Consumable || ItemType == EItemType::IT_Refillable", EditConditionHides))
    FItemAffectData ItemAffectData;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText ToolTip;
};


USTRUCT(BlueprintType)
struct FItemPresentationData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftClassPtr<AWeaponDefaultBase> Bp_Weapon;
};

struct FChargeMotionData
{
    bool Started = false;
    float ElaseTime = 0;
    int32 ChargeStep = 0;
};

USTRUCT(BlueprintType)
struct FIconAtlasData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UTexture2D> Atlas;
    UPROPERTY(Transient)
    TObjectPtr<UTexture2D> LoadedAtlas;
    int32 IconSize;
};

struct FIconData
{
    TObjectPtr<UTexture2D> Atlas = nullptr;
    FVector2D Uv = FVector2D::ZeroVector;
    FVector2D IconSizeNormalized = FVector2D::ZeroVector;
};