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

void ACombinableObject::CreateAddOn(
	UStaticMeshComponent* Parent, 
	ACombinableObject* Piece, 
	FName Name, 
	UStaticMeshComponent* ChildOf) 
{
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
			Piece->GetRootComponent())->GetStaticMesh());

		AddOn->SetCollisionProfileName(FName("PhysicsActor"));
		AddOn->SetNotifyRigidBodyCollision(true);

		Child = AddOn;
		AddDynamicHit(AddOn);
		//Guard->WeldTo(Root, FName("Front"));
		//Guard->SetSimulatePhysics(true);

		Piece->Destroy();
	}
	if (ChildOf != nullptr) {
		UStaticMeshComponent* AddOn2 = NewObject<UStaticMeshComponent>(
			this,
			UStaticMeshComponent::StaticClass(),
			FName("Blade")
			);

		if (AddOn2)
		{
			AddOn2->RegisterComponent();
			AddOn2->SetRelativeLocation(FVector(0, 0, 0));
			AddOn2->SetRelativeRotation(FRotator(0, 0, 0));

			AddOn2->AttachToComponent(Child, FAttachmentTransformRules::KeepRelativeTransform, FName("Blade"));
			AddOn2->SetStaticMesh(ChildOf->GetStaticMesh());

			AddOn2->SetCollisionProfileName(FName("PhysicsActor"));
			AddOn2->SetNotifyRigidBodyCollision(true);

			AddDynamicHit(AddOn2);
		}
	}
}

void ACombinableObject::SetIsReadyToCombine(bool isReady) {
	isReadyToCombine = isReady;
	UE_LOG(LogTemp, Warning, TEXT("%s: Ready to Combine"), *GetName());
}

void ACombinableObject::AddComponent(
	UPrimitiveComponent* HitComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComponent, 
	FVector NormalImpulse, 
	const FHitResult& Hit) {

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
								CreateAddOn(RootComponent, TryCombine, FName("Guard"), TryCombine->Child);
							}
						}
						else if (OtherType == ETypeEnum::TE_Weapon) {
							if (Child) {
								CreateAddOn(Child, TryCombine, FName("Blade"), TryCombine->Child);
							}
							else if(!Child) {
								CreateAddOn(RootComponent, TryCombine, FName("Blade"), TryCombine->Child);
							}
						}
					}
					if (Type == ETypeEnum::TE_Guard) {
						if (OtherType == ETypeEnum::TE_Weapon) {
							if (!Child) {
								CreateAddOn(RootComponent, TryCombine, FName("Blade"), TryCombine->Child);
							}
						}
					}
				}
			}
		//}
	}
}

