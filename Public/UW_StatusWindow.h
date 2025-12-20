// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LikeSoulLikeType.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ComboBoxString.h"
#include "UW_StackableUIBase.h"
#include "UW_StatusWindow.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnEditBarChanged);

class ACharacterPlayableBase;
class UManager_Calculate;
class AWeaponDefaultBase;
UCLASS()
class LIKESOULLIKE_API UUW_StatusWindow : public UUW_StackableUIBase
{
	GENERATED_BODY()
private:
	
	ACharacterPlayableBase * TargetCharacter;
	UManager_Calculate* CalculatManager;
	AWeaponDefaultBase* WeaponSlot1;
	AWeaponDefaultBase* WeaponSlot2;
	AWeaponDefaultBase* WeaponSlot3; 
	
public:
	virtual void InitUI() override;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_StatusEditBox* StatusEditBox;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_WeaponEditBox* WeaponEditBox;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_StateInfoBox* StateBox;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_ResistInfoBox* ResistBox;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_WeaponInfoBox* WeaponSlotBox1;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_WeaponInfoBox* WeaponSlotBox2;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_WeaponInfoBox* WeaponSlotBox3;


	UFUNCTION(BlueprintCallable)
	void ClickedConfirmButton();

	UFUNCTION(BlueprintCallable)
	void ClickedCancleButton(); 
	void OnStatusEdited();
	virtual void NativeConstruct() override;

};

UCLASS()
class LIKESOULLIKE_API UUW_StatusWindow_CalculatInfoBar : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InfoName;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InfoNumber;

	void SettingEditingBar(FString Name, int statenum)
	{
		InfoName->SetText(FText::FromString(Name));
		SetStatusNum(statenum);

	};
	void SetStatusNum(int Num)
	{
		FString temp = FString::FromInt(Num);
		InfoNumber->SetText(FText::FromString(temp));
	}
};
UCLASS()
class LIKESOULLIKE_API UUW_StatusWindow_StatusEditBar : public UUserWidget
{
	GENERATED_BODY()
public:
	int StatusNum;
	FOnEditBarChanged OnEditBarChanged;

	UUW_StatusWindow_StatusEditBox* EditBox;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatusName;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatusNumber;
	UButton* AddButton;
	UButton* Minusutton;
	int InitStateNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString StrStatusName;

	bool IsEdited() { return (InitStateNum != StatusNum ? true : false); };

	void SettingEditingBar(UUW_StatusWindow_StatusEditBox* editBox , FString Name, int statenum)
	{
		EditBox = editBox;
		StatusName->SetText(FText::FromString(Name));
		SetStatusNum(statenum);
		InitStateNum = statenum;
	};

	void SetStatusNum(int Num)
	{
		StatusNum = Num;
		FString temp = FString::FromInt(Num);
		StatusNumber->SetText(FText::FromString(temp));
	};
	int GetStatusNum() { return StatusNum; };

	UFUNCTION(Blueprintcallable)
	void ClickedAddButton();
	UFUNCTION(BlueprintCallable)
	void ClickedMinusButton();
};

UCLASS()
class LIKESOULLIKE_API UUW_StatusWindow_WeaponEditBar : public UUserWidget
{
	GENERATED_BODY()
public:
	int StatusNum;
	int InitStateNum;
	int infusionType;
	int initInfusion;
	FOnEditBarChanged OnEditBarChanged;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* InfusionInfo;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatusNumber;
	UButton* AddButton;
	UButton* Minusutton;
	virtual void NativeConstruct() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString StrStatusName;

	bool IsEdited() { return ((InitStateNum != StatusNum || initInfusion != infusionType)? true : false); };

	int GetUpgrade() { return StatusNum; };
	int GetInfusion() { return InfusionInfo->GetSelectedIndex(); };
	UFUNCTION()
	void UpdatedInfusion(FString SelectedItem, ESelectInfo::Type SelectionType) 
	{
		OnEditBarChanged.Broadcast(); 
	};
	void SetInfusion(EWeaponInfusionType Infusion)
	{
		infusionType = (int)Infusion;
		InfusionInfo->SetSelectedIndex(infusionType);
	};
	void SetUpgrade(int temp) 
	{ 
		StatusNum = temp;
		SetStatusNum(StatusNum);
	};

	void SetStatusNum(int Num)
	{
		FString tempstr = FString::FromInt(Num);
		StatusNumber->SetText(FText::FromString(tempstr));
	};

	UFUNCTION(Blueprintcallable)
	void ClickedAddButton()
	{
		if (StatusNum >= 10) return;
		StatusNum++;
		SetStatusNum(StatusNum); 
		OnEditBarChanged.Broadcast();
	};
	UFUNCTION(BlueprintCallable)
	void ClickedMinusButton()
	{
		if (StatusNum <= InitStateNum) return;
		StatusNum--;
		SetStatusNum(StatusNum);
		OnEditBarChanged.Broadcast();
	};
};


DECLARE_MULTICAST_DELEGATE(FOnEditBoxChanged);
UCLASS()
class LIKESOULLIKE_API UUW_StatusWindow_StatusEditBox : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LevelText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NextRequireSoul;
	int Level;
	virtual void NativeConstruct()override;
	void ModifyLevel(int num) 
	{ 
		Level += num;
		FString temp = FString::FromInt(Level);
		LevelText->SetText(FText::FromString(temp));
	};

	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_StatusEditBar* HealthEditBar;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_StatusEditBar* EnduranceEditBar;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_StatusEditBar* MentalityEditBar;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_StatusEditBar* StrongEditBar;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_StatusEditBar* DexturyEditBar;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_StatusEditBar* IntelligenceEditBar;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_StatusEditBar* FaithEditBar;

	FOnEditBoxChanged OnEditBoxChanged;

	void BindDelegates()
	{
		HealthEditBar->OnEditBarChanged.AddUObject(
			this, &UUW_StatusWindow_StatusEditBox::OnAnyBarEdited);
		EnduranceEditBar->OnEditBarChanged.AddUObject(
			this, &UUW_StatusWindow_StatusEditBox::OnAnyBarEdited);
		MentalityEditBar->OnEditBarChanged.AddUObject(
			this, &UUW_StatusWindow_StatusEditBox::OnAnyBarEdited);
		StrongEditBar->OnEditBarChanged.AddUObject(
			this, &UUW_StatusWindow_StatusEditBox::OnAnyBarEdited);
		DexturyEditBar->OnEditBarChanged.AddUObject(
			this, &UUW_StatusWindow_StatusEditBox::OnAnyBarEdited);
		IntelligenceEditBar->OnEditBarChanged.AddUObject(
			this, &UUW_StatusWindow_StatusEditBox::OnAnyBarEdited);
		FaithEditBar->OnEditBarChanged.AddUObject(
			this, &UUW_StatusWindow_StatusEditBox::OnAnyBarEdited);
	}
	void OnAnyBarEdited()
	{
		OnEditBoxChanged.Broadcast();
	}

	void SetStatusEditBox(FPlayerStatus Info) 
	{
		HealthEditBar->SettingEditingBar(this,"Health", Info.Health);
		EnduranceEditBar->SettingEditingBar(this, "Endurance", Info.Endurance);
		MentalityEditBar->SettingEditingBar(this, "Mentality", Info.Mentality);
		StrongEditBar->SettingEditingBar(this, "Strong", Info.Strong);
		DexturyEditBar->SettingEditingBar(this, "Dextery", Info.Dextery);
		IntelligenceEditBar->SettingEditingBar(this, "Intellegence", Info.Intellegence);
		FaithEditBar->SettingEditingBar(this, "Faith", Info.Faith);
		Level = Info.Level;
		FString temp = FString::FromInt(Info.Level);
		LevelText->SetText(FText::FromString(temp));
	};
	void SetRequireSoul(int nextRequireSoul)
	{
		FString temp = FString::FromInt(nextRequireSoul);
		NextRequireSoul->SetText(FText::FromString(temp));
	};

	void UpdateStatusEditBox(FPlayerStatus Info)
	{
		HealthEditBar->SetStatusNum(Info.Health);
		EnduranceEditBar->SetStatusNum(Info.Endurance);
		MentalityEditBar->SetStatusNum(Info.Mentality);
		StrongEditBar->SetStatusNum(Info.Strong);
		DexturyEditBar->SetStatusNum(Info.Dextery);
		IntelligenceEditBar->SetStatusNum(Info.Intellegence);
		FaithEditBar->SetStatusNum(Info.Faith);
	}

	FPlayerStatus GetEditedStatus()
	{
		FPlayerStatus temp;
		temp.Health = HealthEditBar->GetStatusNum();
		temp.Endurance = EnduranceEditBar->GetStatusNum();
		temp.Mentality = MentalityEditBar->GetStatusNum();
		temp.Strong = StrongEditBar->GetStatusNum();
		temp.Dextery = DexturyEditBar->GetStatusNum();
		temp.Intellegence = IntelligenceEditBar->GetStatusNum();
		temp.Faith = FaithEditBar->GetStatusNum();
		temp.Level = Level;
		return temp;
	}

	bool IsEdited()
	{
		return (HealthEditBar->IsEdited() || EnduranceEditBar->IsEdited() || MentalityEditBar->IsEdited() || 
			StrongEditBar->IsEdited() || DexturyEditBar->IsEdited() || IntelligenceEditBar->IsEdited() || FaithEditBar->IsEdited()) ? true : false;
	}

};
UCLASS()
class LIKESOULLIKE_API UUW_StatusWindow_WeaponEditBox : public UUserWidget
{
	GENERATED_BODY()
public:
	int StatusNum;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_WeaponEditBar* WeaponEditSlot1;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_WeaponEditBar* WeaponEditSlot2;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_WeaponEditBar* WeaponEditSlot3;

	FOnEditBoxChanged OnEditBoxChanged;

	void BindDelegates()
	{
		WeaponEditSlot1->OnEditBarChanged.AddUObject(
			this, &UUW_StatusWindow_WeaponEditBox::OnAnyBarEdited);
		WeaponEditSlot2->OnEditBarChanged.AddUObject(
			this, &UUW_StatusWindow_WeaponEditBox::OnAnyBarEdited);
		WeaponEditSlot3->OnEditBarChanged.AddUObject(
			this, &UUW_StatusWindow_WeaponEditBox::OnAnyBarEdited);
	}
	void OnAnyBarEdited()
	{
		OnEditBoxChanged.Broadcast();
	}

	void SetStatusBox(int slot ,int upgrade,EWeaponInfusionType infusion)
	{
		switch (slot)
		{
		case 1:
			WeaponEditSlot1->SetInfusion(infusion);
			WeaponEditSlot1->SetUpgrade(upgrade);
			break;
		case 2:
			WeaponEditSlot2->SetInfusion(infusion);
			WeaponEditSlot2->SetUpgrade(upgrade);
			break;
		case 3:
			WeaponEditSlot3->SetInfusion(infusion);
			WeaponEditSlot3->SetUpgrade(upgrade);
			break;
		}
	};

	bool GetEditedSlot(int checkslot)
	{
		switch (checkslot)
		{
		case 1:
			return WeaponEditSlot1->IsEdited();
			break;
		case 2:
			return WeaponEditSlot2->IsEdited();
			break;
		case 3:
			return WeaponEditSlot3->IsEdited();
			break;
		default:
			return false;
			break;
		}
	}

	int GetEditedUpgrade(int checkslot)
	{
		switch (checkslot)
		{
		case 1:
			return WeaponEditSlot1->GetUpgrade();
			break;
		case 2:
			return WeaponEditSlot2->GetUpgrade();
			break;
		case 3:
			return WeaponEditSlot3->GetUpgrade();
			break;
		default:
			return 0;
			break;
		}
	}
	int GetEditedInfusion(int checkslot)
	{
		switch (checkslot)
		{
		case 1:
			return WeaponEditSlot1->GetInfusion();
			break;
		case 2:
			return WeaponEditSlot2->GetInfusion();
			break;
		case 3:
			return WeaponEditSlot3->GetInfusion();
			break;
		default:
			return 0;
			break;
		}
	}
};
UCLASS()
class LIKESOULLIKE_API UUW_StatusWindow_WeaponInfoBox : public UUserWidget
{
	GENERATED_BODY()
public:
	int SlotNum;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_CalculatInfoBar* PhysicsDamageBox;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_CalculatInfoBar* DevineDamageBox;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_CalculatInfoBar* LightningDamageBox;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_CalculatInfoBar* MagicDamageBox;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_CalculatInfoBar* FireDamageBox;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SlotNumBlock;
	void SetSlotNum(int slotNum)
	{
		FString temp = FString::FromInt(slotNum);
		SlotNumBlock->SetText(FText::FromString(temp));
	};
	void SetWeaponInfoBox(FDamageData Info)
	{
		PhysicsDamageBox->SettingEditingBar("Physics Dmg", Info.PhysicalDamage);
		DevineDamageBox->SettingEditingBar("Devine Dmg", Info.DevineDamage);
		LightningDamageBox->SettingEditingBar("Lightning Dmg", Info.LightningDamage);
		MagicDamageBox->SettingEditingBar("Magic Dmg", Info.MagicDamage);
		FireDamageBox->SettingEditingBar("Fire Dmg", Info.FireDamage);
	};
};
UCLASS()
class LIKESOULLIKE_API UUW_StatusWindow_ResistInfoBox : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_CalculatInfoBar* PhysicsReistBox;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_CalculatInfoBar* DevineReistBox;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_CalculatInfoBar* LightningReistBox;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_CalculatInfoBar* MagicReistBox;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_CalculatInfoBar* FireReistBox;

	void SetResistInfoBox(FResistData Info)
	{
		PhysicsReistBox->SettingEditingBar("Physics", Info.PhysicalResist);
		DevineReistBox->SettingEditingBar("Devine", Info.DevineResist);
		LightningReistBox->SettingEditingBar("Lightning", Info.LightningResist);
		MagicReistBox->SettingEditingBar("Magic", Info.MagicResist);
		FireReistBox->SettingEditingBar("Fire", Info.FireResist);
	};
};
UCLASS()
class LIKESOULLIKE_API UUW_StatusWindow_StateInfoBox : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_CalculatInfoBar* HPBar;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_CalculatInfoBar* SPBar;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_CalculatInfoBar* MPBar;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_CalculatInfoBar* WeightBar;
	UPROPERTY(meta = (BindWidget))
	UUW_StatusWindow_CalculatInfoBar* ToughnessBar;

	void SetStateInfoBox(FCharacterState Info)
	{
		HPBar->SettingEditingBar("HP", Info.HP);
		SPBar->SettingEditingBar("SP", Info.Stemina);
		MPBar->SettingEditingBar("MP", Info.Mana);
		WeightBar->SettingEditingBar("Weight", Info.Weight);
		ToughnessBar->SettingEditingBar("Toughness", Info.Toughness);
	};
};