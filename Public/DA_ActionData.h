// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LikeSoulLikeType.h"
#include "DA_ActionData.generated.h"

/**

 */
UCLASS()
class LIKESOULLIKE_API UDA_ActionData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EActionType ActionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charge", meta = (EditCondition = "ActionType == EActionType::AT_Charge", EditConditionHides))
	int32 MaxChargeStep = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charge", meta = (EditCondition = "ActionType == EActionType::AT_Charge", EditConditionHides))
	int32 ChargeStepInterval = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charge", meta = (EditCondition = "ActionType == EActionType::AT_Charge", EditConditionHides))
	UCurveFloat* ChargeMotionMutiplyCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsGuardable = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	bool  IsParryable = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool  IsAvoidable = true;



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int		ConsumeStemina	=	0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int		ConsumeMana		=	0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float	MotionDamageMutiply = 0;



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FComboActionData> NextAction;
};
