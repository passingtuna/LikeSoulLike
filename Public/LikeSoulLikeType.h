#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "LikeSoulLikeType.generated.h"


class UDA_ActionData;
class ACharacterDefaultBase;
UENUM(BlueprintType)
enum EItemType
{
    IT_Consumable,IT_Armor,IT_Weapon,IT_KeyItem,IT_Refillable,IT_Material
};

UENUM(BlueprintType)
enum EStatusConditionType
{
    SCT_Poison, SCT_Buff, SCT_Debuff, SCT_Stun
};
UENUM(BlueprintType)
enum EAffectionType
{
    AT_MaxHP, AT_CurrentHp, AT_MaxStemina, AT_CurrentStemina, AT_FireResist, AT_LightningResist, AT_DevineResist, 
    AT_PhysicsDamage, AT_FireDamage, AT_LightningDamage ,AT_MagicDamage , AT_DevineDamage
};

UENUM(BlueprintType)
enum EItemEnchantType
{
    IET_Strong, IET_Dex, IET_StrongDex, IET_Intellegence , IET_Devine
};

UENUM(BlueprintType)
enum ECharacterCamp
{
    CC_Player,CC_Neutral, CC_Monster
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
    int Level           = 15;
    int Health          = 10;
    int Endurance       = 10;
    int Mentality       = 10;
    int Strong          = 10;
    int Dextery         = 10;
    int Intellegence    = 10;
    int Faith           = 10;
};

struct FCharacterState
{
    int HP = 100;
    int Stemina = 100;
    int Mana = 100;
    int Toughness = 50;
    int Weight = 100;
};

struct FResistData
{
    int PhysicalResist  = 0;
    int FireResist      = 0;
    int LightningResist = 0;
    int MagicResist     = 0;
    int DevineResist    = 0;
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
    bool IsGaurdable = 0;
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
    int BaseDamage;
    float MotionMultipler;
};


struct WeaponMeshInitData
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
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int ID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int Count;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int Upgrade;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EWeaponInfusionType Infusion = EWeaponInfusionType::EWIT_None;
};