// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftingSim.h"
#include "Combiner.h"


// Sets default values for this component's properties
UCombiner::UCombiner()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombiner::BeginPlay()
{
	Super::BeginPlay();
	Root = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	if (Root) {
		Root->OnComponentHit.AddDynamic(this, &UCombiner::YourFunction);
		UE_LOG(LogTemp, Warning, TEXT("success"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("failed"));
	}
}


// Called every frame
void UCombiner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
void UCombiner::SetIsReadyToCombine(bool isReady) {
	isReadyToCombine = isReady;
}
void UCombiner::YourFunction(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	
	if (OtherActor->GetClass() == GetOwner()->GetClass()) {
		if (isReadyToCombine)
		{
			UE_LOG(LogTemp, Warning, TEXT("Combine"));
			isReadyToCombine = false;
			UStaticMeshComponent* NewComp = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), FName("AddedMesh"));

			if (NewComp)
			{
				NewComp->RegisterComponent();
				NewComp->SetRelativeLocation(FVector(0, 0, 0));
				NewComp->SetRelativeRotation(FRotator(0, 0, 0));

				NewComp->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform, FName("Front"));
				NewComp->SetStaticMesh(Root->GetStaticMesh());

				NewComp->SetCollisionProfileName(FName("PhysicsActor"));
				NewComp->SetNotifyRigidBodyCollision(true);

				//NewComp->WeldTo(Root, FName("Front"));
				//NewComp->SetSimulatePhysics(true);

				OtherActor->Destroy();
			}
		}
	}
}

