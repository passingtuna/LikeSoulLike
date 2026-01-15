// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager_UI.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "CharacterNonPlayableBase.h"
#include "DA_UIManagerData.h"
#include "UW_LockONHpBar.h"
#include "UW_CharacterState.h"
#include "UW_BossHpBar.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanelSlot.h"
#include "UW_GaugeWidget.h"
#include "UW_UserMenu.h"
#include "CharacterPlayableBase.h"
#include "SoulLikeController.h"
#include "UW_StackableUIBase.h"
#include "UW_QuickSlot.h"
#include "UW_BoneFireMenu.h"
#include "UW_BonefireWarp.h"

void UManager_UI::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    static const FString AssetPath = "/Game/DataAsset/Manager/UIManagerDataAsset.UIManagerDataAsset";
    GameUIDataAsset = Cast<UDA_UIManagerData>(StaticLoadObject(UDA_UIManagerData::StaticClass(), nullptr, *AssetPath));

    if (GameUIDataAsset)
    {
        CharStateWidgetBP = GameUIDataAsset->CharStateWidgetBp;
        BossHPWidgetBP = GameUIDataAsset->BossHPWidgetBP;
        AnnouncementWidgetBp = GameUIDataAsset->AnnouncementWidgetBp;

        UserMenuWidgetBP = GameUIDataAsset->UserMenuWidgetBP;
        StatusWidgetBP = GameUIDataAsset->StatusWidgetBP;


       QuickSlotWidgetBP = GameUIDataAsset->QuickSlotWidgetBP;
       InventoryWidgetBP = GameUIDataAsset->InventoryWidgetBP;
       EquipmentWidgetBP = GameUIDataAsset->EquipmentWidgetBP;

       StorageWidgetBP = GameUIDataAsset->StorageWidgetBP;
       BonfieMenuWidgetBP = GameUIDataAsset->BonfieMenuWidgetBP;

       BonfieWarpMenuWidgetBP = GameUIDataAsset->BonfieWarpMenuWidgetBP;

       AimWidgetBP = GameUIDataAsset->AimWidgetBP;
    }
}

void UManager_UI::InitUI(ACharacterPlayableBase* Character)
{
    PlayerCharacter = Character;
    PlayerController = PlayerCharacter->GetPlayerController();

    if (CharStateWidgetBP)
    {
        CharStateWidget = CreateWidget<UUW_CharacterState>(GetWorld(), CharStateWidgetBP);
        CharStateWidget->AddToViewport(0);
        CharStateWidget->SetVisibility(ESlateVisibility::Visible);
    }
    if (QuickSlotWidgetBP)
    {
        QuickSlotWidget = CreateWidget<UUW_QuickSlot>(GetWorld(), QuickSlotWidgetBP);
        QuickSlotWidget->AddToViewport(0);
        QuickSlotWidget->SetVisibility(ESlateVisibility::Visible);
        QuickSlotWidget->InvisibleItemGetText();
    }
    if (AimWidgetBP)
    {
        AimWidget = CreateWidget<UUserWidget>(GetWorld(), AimWidgetBP);
        AimWidget->AddToViewport(0);
        AimWidget->SetVisibility(ESlateVisibility::Collapsed);
    }


    if (BossHPWidgetBP)
    {
        BossHPWidget = CreateWidget<UUW_BossHpBar>(GetWorld(), BossHPWidgetBP);
        BossHPWidget->AddToViewport(0);
        BossHPWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
    if (AnnouncementWidgetBp)
    {
        AnnouncementWidget = CreateWidget<UUW_Announcement>(GetWorld(), AnnouncementWidgetBp);
        AnnouncementWidget->AddToViewport(100);
        AnnouncementWidget->SetVisibility(ESlateVisibility::Collapsed);
    }


    if (UserMenuWidgetBP)
    {
        UserMenuWidget = CreateWidget<UUW_UserMenu>(GetWorld(), UserMenuWidgetBP);
        UserMenuWidget->AddToViewport(50);
        UserMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
        UserMenuWidget->OnRequestClose.BindUObject(this, &UManager_UI::PopUI);
        UserMenuWidget->InitUserMenu();
    }
    if (StatusWidgetBP)
    {
        StatusWidget = CreateWidget<UUW_StatusWindow>(GetWorld(), StatusWidgetBP);
        StatusWidget->AddToViewport(50);
        StatusWidget->SetVisibility(ESlateVisibility::Collapsed);
        StatusWidget->OnRequestClose.BindUObject(this, &UManager_UI::PopUI);
    }
    if (InventoryWidgetBP)
    {
        InventoryWidget = CreateWidget<UUW_Inventory>(GetWorld(), InventoryWidgetBP);
        InventoryWidget->AddToViewport(50);
        InventoryWidget->InitInventory(PlayerCharacter->GetInventoryComponent());
        InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
        InventoryWidget->OnRequestClose.BindUObject(this, &UManager_UI::PopUI);
    }
    if (EquipmentWidgetBP)
    {
        EquipmentWidget = CreateWidget<UUW_Equipment>(GetWorld(), EquipmentWidgetBP);
        EquipmentWidget->AddToViewport(50);
        EquipmentWidget->InitEquipment(PlayerCharacter);
        EquipmentWidget->SetVisibility(ESlateVisibility::Collapsed);
        EquipmentWidget->OnRequestClose.BindUObject(this, &UManager_UI::PopUI);
    }


    if (StorageWidgetBP)
    {
        StorageWidget = CreateWidget<UUW_Storage>(GetWorld(), StorageWidgetBP);
        StorageWidget->AddToViewport(50);
        StorageWidget->InitStorage(PlayerCharacter);
        StorageWidget->SetVisibility(ESlateVisibility::Collapsed);
        StorageWidget->OnRequestClose.BindUObject(this, &UManager_UI::PopUI);
    }

    if (BonfieMenuWidgetBP)
    {
        BonfieMenuWidget = CreateWidget<UUW_BoneFireMenu>(GetWorld(), BonfieMenuWidgetBP);
        BonfieMenuWidget->AddToViewport(50);
        BonfieMenuWidget->InitBonefireMenu();
        BonfieMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
        BonfieMenuWidget->OnRequestClose.BindUObject(this, &UManager_UI::PopUI);
    }

    if (BonfieWarpMenuWidgetBP)
    {
        BonfieWarpWidget = CreateWidget<UUW_BonefireWarp>(GetWorld(), BonfieWarpMenuWidgetBP);
        BonfieWarpWidget->AddToViewport(50);
        BonfieWarpWidget->InitBonefireWarpWindow(PlayerCharacter);
        BonfieWarpWidget->SetVisibility(ESlateVisibility::Collapsed);
        BonfieWarpWidget->OnRequestClose.BindUObject(this, &UManager_UI::PopUI);
    }

}

void UManager_UI::ShowUIByName(FName uiName)
{
    if (uiName == "UserMenu")
    {
        PushUI(UserMenuWidget);
    }
    else if (uiName == "Status")
    {
        PushUI(StatusWidget);
    }
    else if (uiName == "Equipment")
    {
        PushUI(EquipmentWidget);
    }
    else if (uiName == "Inventory")
    {
        PushUI(InventoryWidget);
    }
    else if (uiName == "Bonefire")
    {
        PushUI(BonfieMenuWidget);
    }
    else if (uiName == "Storage")
    {
        PushUI(StorageWidget);
    }
    else if (uiName == "BonefireWarp")
    {
        PushUI(BonfieWarpWidget);
    }
}
void UManager_UI::UserMenuKeyAction()
{
    PushUI(UserMenuWidget);
}
void UManager_UI::OpenBonefireMenu(FName Name)
{
    BonfieMenuWidget->SetBonefireName(Name);
    ShowUIByName("Bonefire");
}

void UManager_UI::ShowAimWidget(bool On)
{
    if (On)
    {
        AimWidget->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        AimWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
}
void UManager_UI::TakeControlToUI(UUW_StackableUIBase* FocusWidget)
{
    if (PlayerController)
    {
        PlayerController->bShowMouseCursor = true;
        FInputModeUIOnly InputModeData;
        if (FocusWidget)
        {
            InputModeData.SetWidgetToFocus(FocusWidget->TakeWidget()); // 포커스를 UI로
            FocusWidget->SetKeyboardFocus();
        }
        InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // 마우스 자유롭게
        PlayerController->SetInputMode(InputModeData);
    }
}
void UManager_UI::ReturnControlToPlayer()
{
    if (PlayerController)
    {
        FInputModeGameOnly InputModeData;
        PlayerController->SetInputMode(InputModeData);
        PlayerController->bShowMouseCursor = false;
    }
}

void UManager_UI::PushUI(UUW_StackableUIBase* NewUI)
{
    if (!NewUI) return; 
    if (UIStack.Num() > 0 && UIStack.Last() == NewUI)  return;
    if (UIStack.Num() > 0)
    {
        UIStack.Last()->SetVisibility(ESlateVisibility::Collapsed);
    }

    NewUI->SetVisibility(ESlateVisibility::Visible);
    NewUI->InitUI();
    UIStack.Add(NewUI);
    TakeControlToUI(NewUI);
}

void UManager_UI::PopUI()
{
    if (UIStack.Num() == 0) return;
    UUW_StackableUIBase* Top = UIStack.Pop();
    Top->SetVisibility(ESlateVisibility::Collapsed);

    if (UIStack.Num() > 0)
    {
        UUW_StackableUIBase* PopUI = UIStack.Last();
        PopUI->SetVisibility(ESlateVisibility::Visible);
        PopUI->InitUI();
        TakeControlToUI(PopUI);
    }
    else
    {
        ReturnControlToPlayer();
    }
}
void UManager_UI::ClearUI()
{
    if (UIStack.Num() == 0) return;
    UE_LOG(LogTemp, Warning, TEXT("클리어 UI %d"), UIStack.Num());
    int StackNum = UIStack.Num();
    for (int i = 0; i < StackNum; ++i)
    {
        UE_LOG(LogTemp, Warning, TEXT("클리어 UI %d"), i);
        PopUI();
    }

}

UUW_StackableUIBase* UManager_UI::GetTopUI()
{
    return UIStack.Num() > 0 ? UIStack.Last() : nullptr;
}
void UManager_UI::AnnouncementFadeOut(FString Text)
{
    if (AnnouncementWidget)
    {
        AnnouncementWidget->SetVisibility(ESlateVisibility::Visible);
        AnnouncementWidget->SetText(Text);
        AnnouncementWidget->SetFadeOut();
    }
}
void UManager_UI::AnnouncementFadeIn(FString Text)
{
    if (AnnouncementWidget)
    {
        AnnouncementWidget->SetVisibility(ESlateVisibility::Visible);
        AnnouncementWidget->SetText(Text);
        AnnouncementWidget->SetFadeIn();
    }
}

void UManager_UI::AnnouncementText(FString Text)
{
    if (AnnouncementWidget)
    {
        AnnouncementWidget->SetVisibility(ESlateVisibility::Visible);
        AnnouncementWidget->SetText(Text);
        GetWorld()->GetTimerManager().SetTimer(AnounceTimer, [this]() {AnnouncementWidget->SetVisibility(ESlateVisibility::Collapsed); }, 2, false);
    }
}

void UManager_UI::InvisibleAnnouncement()
{
    if (AnnouncementWidget)
    {
        AnnouncementWidget->ResetAnnouncement();
        AnnouncementWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
}


void UManager_UI::SetPlayerHP(float CurrentHP, float MaxHP)
{
	CharStateWidget->HP_GaugeWidget->SetPercent(CurrentHP / MaxHP);
}
void UManager_UI::SetPlayerSP(float CurrentSP, float MaxSP)
{
    CharStateWidget->SP_GaugeWidget->SetPercent(CurrentSP / MaxSP);
}
void UManager_UI::SetPlayerMP(float CurrentMP, float MaxMP)
{
    CharStateWidget->MP_GaugeWidget->SetPercent(CurrentMP / MaxMP);
}

void UManager_UI::ShowBossHPUI(bool On)
{
    On ? BossHPWidget->SetVisibility(ESlateVisibility::Visible) : BossHPWidget->SetVisibility(ESlateVisibility::Collapsed);
}
void UManager_UI::SetBossHP(float CurrentHP, float MaxHP)
{
    BossHPWidget->HP_GaugeWidget->SetPercent(CurrentHP / MaxHP);
}

void UManager_UI::SetBossName(FString Name)
{ 
    BossHPWidget->Boss_Name->SetText(FText::FromString(Name));
}

void UManager_UI::SetPlayerMaxHP(float MaxHP)
{
    float NewWidth = MaxHP;
    if (UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(CharStateWidget->HP_GaugeWidget->Slot))
    {
        Slot->SetSize(FVector2D(NewWidth, Slot->GetSize().Y));
    }
}
void UManager_UI::SetPlayerMaxSP(float MaxSP)
{
    float NewWidth = MaxSP * 2 ;
    if (UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(CharStateWidget->SP_GaugeWidget->Slot))
    {
        Slot->SetSize(FVector2D(NewWidth, Slot->GetSize().Y));
    }
}
void UManager_UI::SetPlayerMaxMP(float MaxMP)
{
    float NewWidth = MaxMP;
    if (UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(CharStateWidget->MP_GaugeWidget->Slot))
    {
        Slot->SetSize(FVector2D(NewWidth, Slot->GetSize().Y));
    }
}


void UManager_UI::ChangeWeaponSlot(FItemData& curWeapon)
{
    QuickSlotWidget->SetWeaponSlot(curWeapon);
}
void UManager_UI::ChangeQuickSlot(FItemData& curItem)
{
    QuickSlotWidget->SetQuickSlot(curItem);
}
void UManager_UI::ChangeSoul(int32 soul)
{
    QuickSlotWidget->SetSoul(soul);
}
void UManager_UI::ShowGetItemUI(FItemData& curItem, FText Text)
{
    QuickSlotWidget->VisibleItemGetText(curItem, Text);
}