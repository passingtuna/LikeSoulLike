// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoneFire.generated.h"

class UHeterogeneousVolumeComponent;
UCLASS()
class LIKESOULLIKE_API ABoneFire : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoneFire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TObjectPtr<USceneComponent> RespawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	FName Name;

	USceneComponent* WarpPosition;
	TArray<UHeterogeneousVolumeComponent*> FirePoint;
	bool IsLitedboneFire;

	void LitBoneFire();
	FTransform GetRespawnTransform();
};
