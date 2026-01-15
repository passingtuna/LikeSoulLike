// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LikeSoulLikeType.h"
#include "NiagaraComponent.h"
#include "DropItem.generated.h"

UCLASS()
class LIKESOULLIKE_API ADropItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UNiagaraComponent* DropSoul;
	UNiagaraComponent* DropItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsSoul;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "IsSoul == true", EditConditionHides))
	int32 Soul;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "IsSoul == false", EditConditionHides))
	FItemData ItemInfo;
	void InitItem();
	void SetDropSoul(int32 Soul);
};
