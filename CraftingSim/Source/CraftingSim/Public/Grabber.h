// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRAFTINGSIM_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	


private:
	//how far the player can reach in cm
	float Reach = 200.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	

	UFUNCTION(BlueprintCallable)
		float GetReach();

	UFUNCTION(BlueprintCallable)
		void Grab();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerGrab();

	UFUNCTION(NetMulticast, Reliable)
		void ClientsGrab(UPrimitiveComponent* ComponentToGrab);


	UFUNCTION(BlueprintCallable)
		void Release();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRelease();

	UFUNCTION(NetMulticast, Reliable)
		void ClientsRelease(UPrimitiveComponent* ComponentToGrab);
		
	void FindPhysicsHandleComponent();
	void FindInputComponent();

	FVector GetReachLineEnd() const;
	FVector GetReachLineStart() const;

	FHitResult GetFirstPhysicsBodyInReach() const;

	UFUNCTION(BlueprintCallable)
	void IncreaseReach();

	UFUNCTION(BlueprintCallable)
	void DecreaseReach();
	
};
