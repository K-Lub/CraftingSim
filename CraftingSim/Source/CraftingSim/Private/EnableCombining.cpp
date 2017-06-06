// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftingSim.h"
#include "EnableCombining.h"
#include "CombinableObject.h"
#include "Combiner.h"


// Sets default values for this component's properties
UEnableCombining::UEnableCombining()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnableCombining::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UEnableCombining::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UEnableCombining::OnBeginTriggerOverlap(class AActor* OtherActor){
	auto* Temp = Cast<ACombinableObject>(OtherActor);
	if (Temp){
		//UE_LOG(LogTemp, Warning, TEXT("Combine"));
		Temp->SetIsReadyToCombine(true);
	}
}

void UEnableCombining::OnEndTriggerOverlap(class AActor* OtherActor) {
	auto* Temp = Cast<ACombinableObject>(OtherActor);
	if (Temp) {
		//UE_LOG(LogTemp, Warning, TEXT("Combine"));
		Temp->SetIsReadyToCombine(false);
	}
}

