// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabProcessComponent.h"
#include "CharacterDefaultBase.h"
#include "DA_DefaultMotion.h"
#include "Components/ArrowComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Manager_Calculate.h"
// Sets default values for this component's properties
UGrabProcessComponent::UGrabProcessComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bEditableWhenInherited = true;

}


// Called when the game starts
void UGrabProcessComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	IsGrabable = false;
	OwnerActor = Cast<ACharacterDefaultBase>(GetOwner());
	IgnoreActors.Add(OwnerActor);
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn)); 
	TArray<UNiagaraComponent*> NiagaraComps;
	GetOwner()->GetComponents<UNiagaraComponent>(NiagaraComps);
	for (UNiagaraComponent* Comp : NiagaraComps)
	{
		if (Comp && Comp->GetName().StartsWith(TEXT("GrapNiagara")))
		{
			NiagaraComp = Comp;
		}
	}
}

// Called every frame
void UGrabProcessComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UGrabProcessComponent::SetGrapReady(bool param)
{
	IsGrabable = param;
	if (NiagaraComp)
	{
		if (IsGrabable)
		{
			NiagaraComp->Activate();
		}
		else
		{
			NiagaraComp->DeactivateImmediate();
		}
	}
}

bool UGrabProcessComponent::CheckGrabProcess(ACharacterDefaultBase* TryActor)
{
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetComponentLocation(),
		120.0f,
		ObjectTypes,
		ACharacterDefaultBase::StaticClass(),
		IgnoreActors,
		GrabReadyActor);

	if (!GrabReadyActor.Contains(TryActor))return false;
	FVector calresult= (OwnerActor->GetActorLocation() - GetComponentLocation()).GetSafeNormal();
	float result = FVector::DotProduct(calresult, TryActor->GetActorForwardVector());
	DrawDebugSphere(GetWorld(), GetComponentLocation(), 120, 16, FColor::Green, false, 0.05f);
	if (result > 0.6f)
	{
		ExcuteGrab(TryActor);
		return true;
	}

	return false;
}

void UGrabProcessComponent::ExcuteGrab(ACharacterDefaultBase* Tryer)
{
	OwnerActor->SetIsGrabable(false);
	NiagaraComp->DeactivateImmediate();
	Tryer->SetActorLocation(GetComponentLocation());
	Tryer->SetActorRotation(GetComponentRotation());
	Tryer->ReadyToAction();
	Tryer->PlayAnimMontage(Tryer->DefaultMotion->GrabMotion);

	//GetWorld()->GetGameInstance()->GetSubsystem<UManager_Calculate>()->Calcu

	FDamageData GrabDamage ;
	GrabDamage.PhysicalDamage = 500;
	OwnerActor->ReceiveAttack(Tryer, GrabDamage, OwnerActor->GetActorForwardVector());
	OwnerActor->ReadyToAction();
	OwnerActor->PlayAnimMontage(OwnerActor->DefaultMotion->GrabedMotion);
}