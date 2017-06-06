// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftingSim.h"
#include "UnrealNetwork.h"


#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include "Grabber.h"

// ...

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bReplicates = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	FindInputComponent();
}




// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	auto LineTraceEnd = GetReachLineEnd();
	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}


void UGrabber::FindPhysicsHandleComponent() {
	//Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("PhysicsHandle not found on %s"), *GetOwner()->GetName());
	}
}
void UGrabber::FindInputComponent() {
	/*InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("InputComponent not found on %s"), *GetOwner()->GetName());
	}*/
}


void UGrabber::Grab() {
	
	if (GetOwnerRole() == ROLE_Authority) {

		auto HitResult = GetFirstPhysicsBodyInReach();
		auto ComponentToGrab = HitResult.GetComponent();
		auto* ActorHit = HitResult.GetActor();
		if (ActorHit)
		{
			//UE_LOG(LogTemp, Error, TEXT("Hit Actor"));
			PhysicsHandle->GrabComponentAtLocationWithRotation(
				ComponentToGrab,
				NAME_None,
				ComponentToGrab->GetOwner()->GetActorLocation(),
				FRotator(0, 0, 0)
				//true
			);
			ClientsGrab(ComponentToGrab);
		}
	}
	if (GetOwnerRole() < ROLE_Authority) {
		ServerGrab();
	}
}
void UGrabber::ServerGrab_Implementation()
{
	Grab();
	return;
}
bool UGrabber::ServerGrab_Validate()
{
	return true;
}
void UGrabber::ClientsGrab_Implementation(UPrimitiveComponent* ComponentToGrab)
{
	if(ComponentToGrab)
		ComponentToGrab->SetEnableGravity(false);
	return;
}


void UGrabber::Release() {

	if (GetOwnerRole() == ROLE_Authority) {
		ClientsRelease(PhysicsHandle->GrabbedComponent);
		PhysicsHandle->ReleaseComponent();
	}
	if (GetOwnerRole() < ROLE_Authority) {
		ServerRelease();
	}
}
void UGrabber::ServerRelease_Implementation()
{
	Release();
	return;
}
bool UGrabber::ServerRelease_Validate()
{
	return true;
}
void UGrabber::ClientsRelease_Implementation(UPrimitiveComponent* ComponentToGrab)
{
	if (ComponentToGrab)
		ComponentToGrab->SetEnableGravity(true);
	return;
}

float UGrabber::GetReach() {
	return Reach;
}

void UGrabber::IncreaseReach() {
	if(Reach < 300)
		Reach += 10;
}

void UGrabber::DecreaseReach() {
	if(Reach > 150)
		Reach -= 10;
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{

	FHitResult Hit;

	//linetrace for objects
	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner())

	);
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
		UE_LOG(LogTemp, Warning, TEXT("%s hit"), *(Hit.GetActor()->GetName()));

	return Hit;
}

FVector UGrabber::GetReachLineStart() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	auto Con = Cast<ACharacter>(GetOwner());
	Con->Controller->GetPlayerViewPoint
	//GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		PlayerViewPointLocation,
		PlayerViewPointRotation
	);
	return PlayerViewPointLocation;

}

FVector UGrabber::GetReachLineEnd() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	auto Con = Cast<ACharacter>(GetOwner());
	auto Con2 = Con->Controller;
	if(Con2){
	Con2->GetPlayerViewPoint
		//GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
		(
			PlayerViewPointLocation,
			PlayerViewPointRotation
		);
	}

	return PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

}

