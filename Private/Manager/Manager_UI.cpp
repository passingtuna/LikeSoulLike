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


void UManager_UI::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    static const FString AssetPath = "/Game/DataAsset/Manager/UIManagerDataAsset.UIManagerDataAsset";
    GameUIDataAsset = Cast<UDA_UIManagerData>(StaticLoadObject(UDA_UIManagerData::StaticClass(), nullptr, *AssetPath));

    if (GameUIDataAsset)
    {
        CharStateWidgetBP = GameUIDataAsset->CharStateWidgetBp;
        BossHPWidgetBP = GameUIDataAsset->BossHPWidgetBP;
        YouDiedWidgetBp = GameUIDataAsset->YouDiedWidgetBp;
        StatusWidgetBP = GameUIDataAsset->StatusWidgetBP;
        UserMenuWidgetBP = GameUIDataAsset->UserMenuWidgetBP;
    }
}

void UManager_UI::InitUI(ACharacterPlayableBase* Character)
{
    if (CharStateWidgetBP)
    {
        CharStateWidget = CreateWidget<UUW_CharacterState>(GetWorld(), CharStateWidgetBP);
        CharStateWidget->AddToViewport(0);
        CharStateWidget->SetVisibility(ESlateVisibility::Visible);
    }

    if (BossHPWidgetBP)
    {
        BossHPWidget = CreateWidget<UUW_BossHpBar>(GetWorld(), BossHPWidgetBP);
        BossHPWidget->AddToViewport(0);
        BossHPWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (YouDiedWidgetBp)
    {
        YouDiedWidget = CreateWidget<UUW_YouDied>(GetWorld(), YouDiedWidgetBp);
        YouDiedWidget->AddToViewport(100);
        YouDiedWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (StatusWidgetBP)
    {
        StatusWidget = CreateWidget<UUW_StatusWindow>(GetWorld(), StatusWidgetBP);
        StatusWidget->AddToViewport(50);
        StatusWidget->SetVisibility(ESlateVisibility::Collapsed);
        StatusWidget->OnRequestClose.BindUObject( this, &UManager_UI::PopUI);
    }

    if (UserMenuWidgetBP)
    {
        UserMenuWidget = CreateWidget<UUW_UserMenu>(GetWorld(), UserMenuWidgetBP);
        UserMenuWidget->AddToViewport(50);
        UserMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
        UserMenuWidget->OnRequestClose.BindUObject(this, &UManager_UI::PopUI);
        UserMenuWidget->InitUserMenu();
    }

    PlayerCharacter = Character;
    PlayerController = PlayerCharacter->GetPlayerController();
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
}
void UManager_UI::UserMenuKeyAction()
{
    PushUI(UserMenuWidget);
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

UUW_StackableUIBase* UManager_UI::GetTopUI()
{
    return UIStack.Num() > 0 ? UIStack.Last() : nullptr;
}

void UManager_UI::SetDeadUI()
{
    if (YouDiedWidget)
    {
        YouDiedWidget->SetVisibility(ESlateVisibility::Visible);
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

void UManager_UI::SetPlayerMaxHP(float MaxHP)
{
    float NewWidth = MaxHP;
	UE_LOG(LogTemp, Warning, TEXT("New HP Gauge Width: %f"), NewWidth);
    if (UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(CharStateWidget->HP_GaugeWidget->Slot))
    {
        Slot->SetSize(FVector2D(NewWidth, Slot->GetSize().Y));
    }
}
void UManager_UI::SetPlayerMaxSP(float MaxSP)
{
    float NewWidth = MaxSP * 2 ;
	UE_LOG(LogTemp, Warning, TEXT("New SP Gauge Width: %f"), NewWidth);
    if (UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(CharStateWidget->SP_GaugeWidget->Slot))
    {
        Slot->SetSize(FVector2D(NewWidth, Slot->GetSize().Y));
    }
}
void UManager_UI::SetPlayerMaxMP(float MaxMP)
{
    float NewWidth = MaxMP;
	UE_LOG(LogTemp, Warning, TEXT("New MP Gauge Width: %f"), NewWidth);
    if (UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(CharStateWidget->MP_GaugeWidget->Slot))
    {
        Slot->SetSize(FVector2D(NewWidth, Slot->GetSize().Y));
    }
}