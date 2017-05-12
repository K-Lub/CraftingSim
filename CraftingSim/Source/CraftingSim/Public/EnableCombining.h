// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "EnableCombining.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCombineRequest);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRAFTINGSIM_API UEnableCombining : public UActorComponent
{
	GENERATED_BODY()
		//refresh senpai

public:	
	// Sets default values for this component's properties
	UEnableCombining();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//UPROPERTY(BlueprintAssignable)
		//FOnCombineRequest OnCombineRequest;

private:
	UPROPERTY(EditAnywhere)
	ATriggerVolume* AreaVolume;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> ActorsOnTable;
		

	
};
