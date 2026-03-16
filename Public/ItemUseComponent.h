#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemUseComponent.generated.h"

class ACharacterPlayableBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIKESOULLIKE_API UItemUseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UItemUseComponent();

	void Initialize(ACharacterPlayableBase* InOwner);

	void HandleUseItem();

private:
	UPROPERTY()
	TObjectPtr<ACharacterPlayableBase> OwnerChar;
};

