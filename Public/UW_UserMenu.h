
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_StackableUIBase.h"
#include "UW_UserMenu.generated.h"

/**
 *
 */
class UManager_UI;
UCLASS()
class LIKESOULLIKE_API UUW_UserMenu : public UUW_StackableUIBase
{
	GENERATED_BODY()
public:
	void InitUserMenu();
	UManager_UI* UIManager;
	UFUNCTION(BlueprintCallable)
	void ClickedInventoryButton();
	UFUNCTION(BlueprintCallable)
	void ClickedQuickSlotButton();
	UFUNCTION(BlueprintCallable)
	void ClickedOptionButton();
};
