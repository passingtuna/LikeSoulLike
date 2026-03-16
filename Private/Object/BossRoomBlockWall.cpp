// Fill out your copyright notice in the Description page of Project Settings.


#include "BossRoomBlockWall.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CharacterNonPlayableBase.h"
#include "CharacterPlayableBase.h"
#include "Base_AIController.h"
#include "Manager_UI.h"

// Sets default values
ABossRoomBlockWall::ABossRoomBlockWall()
{
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ABossRoomBlockWall::BeginPlay()
{
	Super::BeginPlay();

    PlaceVolume = GetComponentByClass<UBoxComponent>();
    Wall = GetComponentByClass<UStaticMeshComponent>();

    SetMeshState(false);
    PlaceVolume->OnComponentBeginOverlap.AddDynamic(this, &ABossRoomBlockWall::OnOverlapBegin);
}

// Called every frame
void ABossRoomBlockWall::Tick(float DeltaTime)
{
}


void ABossRoomBlockWall::SetMeshState(bool state)
{
    if (!Wall) return;
    Wall->SetVisibility(state);
    FName TempName;
    state ? TempName = "BlockAllDynamic" : TempName = "NoCollision";
    Wall->SetCollisionProfileName(TempName);
}

void ABossRoomBlockWall::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ACharacterPlayableBase* Char = Cast<ACharacterPlayableBase>(OtherActor);
    if (Char)
    {
        if (BossChar&& !BossChar->GetIsDead())
        {
            SetMeshState(true);
            BossChar->OnDeadDeligate.BindLambda([this]() 
                {
                    SetMeshState(false);
                    GetWorld()->GetGameInstance()->GetSubsystem<UManager_UI>()->ShowBossHPUI(false); 
                    GetWorld()->GetGameInstance()->GetSubsystem<UManager_UI>()->AnnouncementText("HEIR OF FIRE DESTROYED");
                });
            BossChar->OnRespawnDeligate.BindLambda([this]() {SetMeshState(false);});
            BossChar->BaseController->SetTarget(Char);
            BossChar->BossEngage();
        }
    }
}
