#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DeathRespawnComponent.generated.h"

class ACharacterPlayableBase;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAnnouncementFadeOut, FString /*Text*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBossHPUI, bool /*On*/);
DECLARE_MULTICAST_DELEGATE(FOnRequestInvisibleAnnouncement);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIKESOULLIKE_API UDeathRespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDeathRespawnComponent();

	void Initialize(ACharacterPlayableBase* InOwner);

	void HandleDeath();

	FOnAnnouncementFadeOut OnAnnouncementFadeOut;
	FOnBossHPUI OnBossHPUI;
	FOnRequestInvisibleAnnouncement OnRequestInvisibleAnnouncement;

private:
	void HandleRespawnAfterDelay();

private:
	UPROPERTY()
	TObjectPtr<ACharacterPlayableBase> OwnerChar;
};

