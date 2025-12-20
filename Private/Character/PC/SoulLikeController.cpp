// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulLikeController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "DA_IAContainer.h"
#include "CharacterPlayableBase.h"

void ASoulLikeController::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PC = Cast<APlayerController>(this))
	{
		if (ULocalPlayer* LP = PC->GetLocalPlayer())
		{
			UEnhancedInputLocalPlayerSubsystem* inputSubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP);
			inputSubSystem->AddMappingContext(IMC_MoveDefault, 0);
		}
	}
}

void ASoulLikeController::SetupInputComponent()
{
	Super::SetupInputComponent();
	BindActionWithIAContainer();
}


void ASoulLikeController::BindActionWithIAContainer()
{
	UEnhancedInputComponent* EhancedIputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	for (const auto& Elem : IAContainer_Move->Actions)
	{
		UInputAction* Action = Elem.Value;
		EhancedIputComponent->BindAction(Action, ETriggerEvent::Started,	this, &ASoulLikeController::OnActionStarted);
		EhancedIputComponent->BindAction(Action, ETriggerEvent::Triggered,	this, &ASoulLikeController::OnActionTriggered);
		EhancedIputComponent->BindAction(Action, ETriggerEvent::Completed,	this, &ASoulLikeController::OnActionCompleted);
	}
}
//내용은 똑같은데 분리하지 않으면 특정 트리거가 씹히는 엔진버그가 있어서 분리함
void ASoulLikeController::OnActionStarted(const FInputActionInstance& instance)
{
	ACharacterPlayableBase* possesedChar = Cast<ACharacterPlayableBase>(GetCharacter());
	const UInputAction* SourceAction = instance.GetSourceAction();
	if (!SourceAction) return;
	FName ActionName = IAContainer_Move->FindNameByAction(SourceAction);
	if (ActionName == "NULL") return;
	FInputActionValue Value = instance.GetValue();
	//ETriggerEvent TriggerEvent = instance.GetTriggerEvent();

	if (!SourceAction) return;
	if (possesedChar)
	{
		possesedChar->HandleInput(ActionName, ETriggerEvent::Started, instance);
	}
}
void ASoulLikeController::OnActionTriggered(const FInputActionInstance& instance)
{
	ACharacterPlayableBase* possesedChar = Cast<ACharacterPlayableBase>(GetCharacter());
	const UInputAction* SourceAction = instance.GetSourceAction();
	if (!SourceAction) return;
	FName ActionName = IAContainer_Move->FindNameByAction(SourceAction);
	if (ActionName == "NULL") return;
	FInputActionValue Value = instance.GetValue();

	if (!SourceAction) return;
	if (possesedChar)
	{
		possesedChar->HandleInput(ActionName, ETriggerEvent::Triggered, instance);
	}
}

void ASoulLikeController::OnActionCompleted(const FInputActionInstance& instance)
{
	ACharacterPlayableBase* possesedChar = Cast<ACharacterPlayableBase>(GetCharacter());
	const UInputAction* SourceAction = instance.GetSourceAction();
	if (!SourceAction) return;
	FName ActionName = IAContainer_Move->FindNameByAction(SourceAction);
	if (ActionName == "NULL") return;

	if (!SourceAction) return;
	if (possesedChar)
	{
		possesedChar->HandleInput(ActionName, ETriggerEvent::Completed, instance);
	}

}

