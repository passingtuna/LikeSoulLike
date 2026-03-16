#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LikeSoulLikeType.h"
#include "PlayerStatusComponent.generated.h"

class ACharacterPlayableBase;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPlayerGaugeChanged, float /*Current*/, float /*Max*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerSoulChanged, int32 /*Soul*/);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIKESOULLIKE_API UPlayerStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerStatusComponent();

	void Initialize(ACharacterPlayableBase* InOwner);

	FOnPlayerGaugeChanged OnHPChanged;
	FOnPlayerGaugeChanged OnSPChanged;
	FOnPlayerGaugeChanged OnMPChanged;
	FOnPlayerSoulChanged OnSoulChanged;

	void OnHealthChanged();
	void OnSteminaChanged();
	void OnManaChanged();

	void AddSoul(int32 DeltaSoul);
	void BroadcastAll();

private:
	UPROPERTY()
	TObjectPtr<ACharacterPlayableBase> OwnerChar;
};

