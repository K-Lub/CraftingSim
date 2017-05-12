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
	AreaVolume->GetOverlappingActors(ActorsOnTable);
	for (AActor* Actor : ActorsOnTable) {
		//OnCombineRequest.Broadcast();
		ACombinableObject* Combiner = Cast<ACombinableObject>(Actor);
		if(Combiner)
			Combiner->SetIsReadyToCombine(true);
	}
}

