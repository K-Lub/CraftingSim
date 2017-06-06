// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftingSim.h"
#include "CombinableObject.h"


// Sets default values
ACombinableObject::ACombinableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;
	//RootComponent->DestroyComponent();
	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	TempComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TempMesh"));
	TempComponent->AttachToComponent(
		RootComponent, 
		FAttachmentTransformRules::KeepRelativeTransform, 
		FName("Test")
	);
	AddDynamicHit(RootComponent);
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



void ACombinableObject::AddDynamicHit_Implementation(UStaticMeshComponent* Comp) {
	Comp->OnComponentHit.AddDynamic(this, &ACombinableObject::AddComponent);
}

bool ACombinableObject::AddDynamicHit_Validate(UStaticMeshComponent* Comp) {
	return true;
}

void ACombinableObject::CreateAddOn_Implementation(UStaticMeshComponent* Parent,ACombinableObject* Piece,FName Name)
{
	ClientsAddComponent(Parent, Piece, Name);
}
bool ACombinableObject::CreateAddOn_Validate(UStaticMeshComponent* Parent, ACombinableObject* Piece, FName Name) {
	return true;
}


void ACombinableObject::ClientsAddComponent_Implementation(
	UStaticMeshComponent* Parent,
	ACombinableObject* Piece,
	FName Name) 
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


		if (Type == ETypeEnum::TE_ChairBase && Parent->DoesSocketExist(Name)) {
			AddOn->AttachToComponent(Parent, FAttachmentTransformRules::KeepRelativeTransform, Name);

			AddOn->SetStaticMesh(Cast<UStaticMeshComponent>(
				Piece->GetRootComponent())->GetStaticMesh());

			AddOn->SetCollisionProfileName(FName("PhysicsActor"));
			AddOn->SetNotifyRigidBodyCollision(true);

			if (Piece->GetRootComponent()->GetChildComponent(0)) {
				UStaticMeshComponent* Temp = Cast<UStaticMeshComponent>(Piece->RootComponent->GetChildComponent(0));
				ClientsAddComponentHelper(
					AddOn,
					Temp,
					Temp->GetFName()
				);
			}
			AddDynamicHit(AddOn);

			Piece->Destroy();
			return;
		}
		for (auto* AttachToComp = Parent;
			AttachToComp != nullptr;
			AttachToComp = Cast<UStaticMeshComponent>(AttachToComp->GetChildComponent(0)))
		{
			if (!AttachToComp->GetChildComponent(0) && AttachToComp->DoesSocketExist(Name)) {

				//attaching to correct socket
				AddOn->AttachToComponent(AttachToComp, FAttachmentTransformRules::KeepRelativeTransform, Name);

				//setting to correct mesh
				AddOn->SetStaticMesh(Cast<UStaticMeshComponent>(
					Piece->GetRootComponent())->GetStaticMesh());

				//adding phsyics/collision
				AddOn->SetCollisionProfileName(FName("PhysicsActor"));
				AddOn->SetNotifyRigidBodyCollision(true);
				AddDynamicHit(AddOn);

				//set our parent object to face upwards so no clipping happens in ground
				this->SetActorRotation(FRotator(0, GetActorRotation().Yaw, 0));
				

				//if we need to add more components
				
				if (Piece->GetRootComponent()->GetChildComponent(0)) {
					UStaticMeshComponent* Temp = Cast<UStaticMeshComponent>(Piece->GetRootComponent()->GetChildComponent(0));
					if (Temp) {
						ClientsAddComponentHelper(
							AddOn,
							Temp,
							Temp->GetFName()
						);
					}
				}
				
				Piece->Destroy();
				return;
			}
		}
	}
}

void ACombinableObject::ClientsAddComponentHelper_Implementation(
	UStaticMeshComponent* Parent,
	UStaticMeshComponent* Piece,
	FName Name)
{
	if (Parent && Piece){
		UStaticMeshComponent* AddOn2 = NewObject<UStaticMeshComponent>(
			this,
			UStaticMeshComponent::StaticClass(),
			Name
			);

		if (AddOn2)
		{
			AddOn2->RegisterComponent();
			AddOn2->SetRelativeLocation(FVector(0, 0, 0));
			AddOn2->SetRelativeRotation(FRotator(0, 0, 0));

			AddOn2->AttachToComponent(Parent, FAttachmentTransformRules::KeepRelativeTransform, Name);
			AddOn2->SetStaticMesh(Piece->GetStaticMesh());

			AddOn2->SetCollisionProfileName(FName("PhysicsActor"));
			AddOn2->SetNotifyRigidBodyCollision(true);

			if (Piece->GetChildComponent(0)) {
				UStaticMeshComponent* Temp = Cast<UStaticMeshComponent>(Piece->GetChildComponent(0));
				ClientsAddComponentHelper(
					AddOn2,
					Temp,
					Temp->GetFName()
				);
			}
			AddDynamicHit(AddOn2);
		}
	}
}



void ACombinableObject::SetIsReadyToCombine(bool isReady) {
	isReadyToCombine = isReady;
}

void ACombinableObject::AddComponent_Implementation(
	UPrimitiveComponent* HitComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComponent, 
	FVector NormalImpulse, 
	const FHitResult& Hit) 
{//Function Portion
	if(isReadyToCombine){
		ACombinableObject* TryCombine = Cast<ACombinableObject>(OtherActor);
		if (TryCombine && TryCombine->isReadyToCombine)
		{
			auto OtherType = TryCombine->Type;
			if (isReadyToCombine && TryCombine->isReadyToCombine)
			{

				//isReadyToCombine = false;

				if (Type != TryCombine->Type) {
					if (Type == ETypeEnum::TE_Handle) {
						if (OtherType == ETypeEnum::TE_Guard) {
							CreateAddOn(RootComponent, TryCombine, FName("Guard"));
						}
						else if (OtherType == ETypeEnum::TE_Weapon) {
							CreateAddOn(RootComponent, TryCombine, FName("Blade"));
						}
					}
					if (Type == ETypeEnum::TE_Guard) {
						if (OtherType == ETypeEnum::TE_Weapon) {
							CreateAddOn(RootComponent, TryCombine, FName("Blade"));
						}
					}
					if (Type == ETypeEnum::TE_ChairBase) {
						if (OtherType == ETypeEnum::TE_ChairLeg) {
							switch (AttachedLegs) {
							case 0:
								CreateAddOn(RootComponent, TryCombine, FName("Leg_BL"));
								AttachedLegs++;
								break;
							case 1:
								CreateAddOn(RootComponent, TryCombine, FName("Leg_BR"));
								AttachedLegs++;
								break;
							case 2:
								CreateAddOn(RootComponent, TryCombine, FName("Leg_FL"));
								AttachedLegs++;
								break;
							case 3:
								CreateAddOn(RootComponent, TryCombine, FName("Leg_FR"));
								AttachedLegs++;
								break;
							}
						}
						else if (OtherType == ETypeEnum::TE_ChairBack) {
							CreateAddOn(RootComponent, TryCombine, FName("Back"));
						}
					}
				}
			}
		}
	}
}
bool ACombinableObject::AddComponent_Validate(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	return true;
}

