#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LikeSoulLikeType.h"
#include "InteractComponent.generated.h"

class ACharacterPlayableBase;
class ADropItem;
class ABoneFire;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnShowGetItemUI, FItemData /*Item*/, FText /*Text*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAnnouncementText, FString /*Text*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnOpenBonefireMenu, FName /*BonefireName*/);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIKESOULLIKE_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractComponent();

	void Initialize(ACharacterPlayableBase* InOwner);

	// 플레이어 입력에서 바로 호출
	void HandleInteract();

	// UI가 구독하는 이벤트
	FOnShowGetItemUI OnShowGetItemUI;
	FOnAnnouncementText OnAnnouncementText;
	FOnOpenBonefireMenu OnOpenBonefireMenu;

private:
	void SearchAndExecute();
	void HandleDropItem(ADropItem* DropItem);
	void HandleBoneFire(ABoneFire* BoneFire);

private:
	UPROPERTY()
	TObjectPtr<ACharacterPlayableBase> OwnerChar;

	UPROPERTY(EditAnywhere, Category = "Interact")
	float SearchRadius = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Interact")
	float ForwardOffset = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Interact")
	float ZOffset = -50.0f;
};

