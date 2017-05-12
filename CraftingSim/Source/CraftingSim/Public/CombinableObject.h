// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CombinableObject.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ETypeEnum : uint8
{
	TE_Handle 	UMETA(DisplayName = "Handle"),
	TE_Guard 	UMETA(DisplayName = "Guard"),
	TE_Weapon 	UMETA(DisplayName = "Weapon")
};

UCLASS()
class CRAFTINGSIM_API ACombinableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACombinableObject();

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

	UStaticMeshComponent* Child;

	UStaticMeshComponent* RootComponent;	

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* TempComponent;

	UPROPERTY(EditAnywhere, Category = Setup)
	ETypeEnum Type;

	void AddDynamicHit(UStaticMeshComponent* Comp);

	void CreateAddOn(UStaticMeshComponent* Parent, ACombinableObject* TryCombine, FName Name);
};
