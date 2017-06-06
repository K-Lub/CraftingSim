// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CombinableObject.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ETypeEnum : uint8
{
	TE_Handle 		UMETA(DisplayName = "Handle"),
	TE_Guard 		UMETA(DisplayName = "Guard"),
	TE_Weapon 		UMETA(DisplayName = "Weapon"),
	TE_ChairBase 	UMETA(DisplayName = "Chair Base"),
	TE_ChairLeg		UMETA(DisplayName = "Chair Leg"),
	TE_ChairBack 	UMETA(DisplayName = "Chair Back")
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
	
	void SetIsReadyToCombine(bool isReady);

private:

	bool isReadyToCombine = false;

	int AttachedLegs = 0;

	UStaticMeshComponent* RootComponent;	

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* TempComponent;

	UPROPERTY(EditAnywhere, Category = Setup)
	ETypeEnum Type;

	UFUNCTION(Server, Reliable, WithValidation)
	void AddComponent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(Server, Reliable, WithValidation)
	void AddDynamicHit(UStaticMeshComponent* Comp);

	UFUNCTION(Server, Reliable, WithValidation)
	void CreateAddOn(UStaticMeshComponent* Parent, ACombinableObject* Piece, FName Name);


	UFUNCTION(NetMulticast, Unreliable)
	void ClientsAddComponent(UStaticMeshComponent* Parent, ACombinableObject* Piece, FName Name);

	UFUNCTION(NetMulticast, Unreliable)
		void ClientsAddComponentHelper(UStaticMeshComponent* Parent, UStaticMeshComponent* Piece, FName Name);
};
