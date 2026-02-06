// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossRoomBlockWall.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class ACharacterNonPlayableBase;
UCLASS()
class LIKESOULLIKE_API ABossRoomBlockWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossRoomBlockWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* PlaceVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Wall;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TObjectPtr<ACharacterNonPlayableBase> BossChar;

	void SetMeshState(bool state);
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);

};
