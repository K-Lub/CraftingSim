// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftingSim.h"
#include "CuttableObject.h"


// Sets default values
ACuttableObject::ACuttableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACuttableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACuttableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACuttableObject::AddComponent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit){}

void ACuttableObject::SetIsReadyToCombine(bool isReady) {}

void ACuttableObject::AddDynamicHit(UStaticMeshComponent* Comp){}

void ACuttableObject::CreateAddOn(UStaticMeshComponent*, ACuttableObject*, FName){}
void ACuttableObject::CreateAddOnHelper(UStaticMeshComponent*, UStaticMeshComponent*, FName){}
