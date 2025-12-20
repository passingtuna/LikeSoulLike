// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SoulLikeController.generated.h"


class UInputAction;
class UInputMappingContext;
class UDA_IAContainer;
class UEnhancedInputComponent;
struct FInputActionInstance;

UCLASS()
class LIKESOULLIKE_API ASoulLikeController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputControl")
	UInputMappingContext * IMC_MoveDefault;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputControl")
	UDA_IAContainer* IAContainer_Move;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputControl")
	UInputMappingContext* IMC_UIDefault;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputControl")
	UDA_IAContainer* IAContainer_UI;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void BindActionWithIAContainer();
	void OnActionStarted(const FInputActionInstance& Value);
	void OnActionTriggered(const FInputActionInstance& Value);
	void OnActionCompleted(const FInputActionInstance& Value);
};
