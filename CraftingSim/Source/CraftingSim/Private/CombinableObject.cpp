// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftingSim.h"
#include "CombinableObject.h"


// Sets default values
ACombinableObject::ACombinableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//RootComponent->DestroyComponent();
	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	/*static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		RootComponent->SetStaticMesh(SphereVisualAsset.Object);
	}*/

	TempComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TempMesh"));
	TempComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform, FName("Test"));
	}

// Called when the game starts or when spawned
void ACombinableObject::BeginPlay()
{
	Super::BeginPlay();
	
	RootComponent->SetStaticMesh(TempComponent->GetStaticMesh());
	TempComponent->DestroyComponent();
	RootComponent->SetCollisionProfileName(FName("PhysicsActor"));
	RootComponent->SetNotifyRigidBodyCollision(true);
	RootComponent->SetSimulatePhysics(true);
	AddDynamicHit(RootComponent);
	
	
}

// Called every frame
void ACombinableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACombinableObject::AddDynamicHit(UStaticMeshComponent* Comp) {
	Comp->OnComponentHit.AddDynamic(this, &ACombinableObject::AddComponent);
}

void ACombinableObject::CreateAddOn(UStaticMeshComponent* Parent, ACombinableObject* TryCombine, FName Name) {
	UStaticMeshComponent* AddOn = NewObject<UStaticMeshComponent>(
		this,
		UStaticMeshComponent::StaticClass(),
		Name
		);

	if (AddOn)
	{
		AddOn->RegisterComponent();
		AddOn->SetRelativeLocation(FVector(0, 0, 0));
		AddOn->SetRelativeRotation(FRotator(0, 0, 0));

		AddOn->AttachToComponent(Parent, FAttachmentTransformRules::KeepRelativeTransform, Name);
		AddOn->SetStaticMesh(Cast<UStaticMeshComponent>(
			TryCombine->GetRootComponent())->GetStaticMesh());

		AddOn->SetCollisionProfileName(FName("PhysicsActor"));
		AddOn->SetNotifyRigidBodyCollision(true);

		Child = AddOn;
		//Guard->WeldTo(Root, FName("Front"));
		//Guard->SetSimulatePhysics(true);

		TryCombine->Destroy();
	}
}

void ACombinableObject::SetIsReadyToCombine(bool isReady) {
	isReadyToCombine = isReady;
	UE_LOG(LogTemp, Warning, TEXT("%s: Ready to Combine"), *GetName());
}

void ACombinableObject::AddComponent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {

	//if (OtherActor->GetClass() == TSubclassOf<ACombinableObject>()) {
		
		ACombinableObject* TryCombine = Cast<ACombinableObject>(OtherActor);
		if (TryCombine)
		{
			UE_LOG(LogTemp, Warning, TEXT("Combine"));
			auto OtherType = TryCombine->Type;
			if (isReadyToCombine && TryCombine->isReadyToCombine)
			{
				
				//isReadyToCombine = false;

				if (Type != TryCombine->Type) {
					if (Type == ETypeEnum::TE_Handle) {
						if (OtherType == ETypeEnum::TE_Guard) {
							if(!Child){
								CreateAddOn(RootComponent, TryCombine, FName("Guard"));
							}
						}
						else if (OtherType == ETypeEnum::TE_Weapon) {
							if (Child) {
								UStaticMeshComponent* AddOn = NewObject<UStaticMeshComponent>(
									this,
									UStaticMeshComponent::StaticClass(),
									FName("Weapon")
									);

								if (AddOn)
								{
									AddOn->RegisterComponent();
									AddOn->SetRelativeLocation(FVector(0, 0, 0));
									AddOn->SetRelativeRotation(FRotator(0, 0, 0));

									AddOn->AttachToComponent(Child, FAttachmentTransformRules::KeepRelativeTransform, FName("Sword"));
									AddOn->SetStaticMesh(Cast<UStaticMeshComponent>(
										TryCombine->GetRootComponent())->GetStaticMesh());

									AddOn->SetCollisionProfileName(FName("PhysicsActor"));
									AddOn->SetNotifyRigidBodyCollision(true);

									Child = AddOn;

									//Guard->WeldTo(Root, FName("Front"));
									//Guard->SetSimulatePhysics(true);

									OtherActor->Destroy();
								}
							}
							else if(!Child) {
								UStaticMeshComponent* AddOn = NewObject<UStaticMeshComponent>(
									this,
									UStaticMeshComponent::StaticClass(),
									FName("Weapon")
									);

								if (AddOn)
								{
									AddOn->RegisterComponent();
									AddOn->SetRelativeLocation(FVector(0, 0, 0));
									AddOn->SetRelativeRotation(FRotator(0, 0, 0));

									AddOn->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform, FName("Sword"));
									AddOn->SetStaticMesh(Cast<UStaticMeshComponent>(
										TryCombine->GetRootComponent())->GetStaticMesh());

									AddOn->SetCollisionProfileName(FName("PhysicsActor"));
									AddOn->SetNotifyRigidBodyCollision(true);

									Child = AddOn;

									//Guard->WeldTo(Root, FName("Front"));
									//Guard->SetSimulatePhysics(true);

									OtherActor->Destroy();
								}
							}
						}
					}
				}
			}
		//}
	}
}

