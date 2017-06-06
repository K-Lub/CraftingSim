// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CuttableObject.generated.h"

UCLASS()
class CRAFTINGSIM_API ACuttableObject : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ACuttableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void AddComponent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	void SetIsReadyToCombine(bool isReady);

private:

	bool isReadyToCombine = false;

	int AttachedLegs = 0;

	UStaticMeshComponent* RootComponent;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* TempComponent;

	void AddDynamicHit(UStaticMeshComponent* Comp);

	void CreateAddOn(UStaticMeshComponent*, ACuttableObject*, FName);
	void CreateAddOnHelper(UStaticMeshComponent*, UStaticMeshComponent*, FName);
};
