// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "TableSaw.generated.h"

/**
 * 
 */

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRAFTINGSIM_API UTableSaw : public UActorComponent
{
	GENERATED_BODY()
		//refresh senpai

public:
	// Sets default values for this component's properties
	UTableSaw();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool Power = false;

private:
	UPROPERTY(EditAnywhere)
		ATriggerVolume* AreaVolume;

	UPROPERTY(VisibleAnywhere)
		TArray<AActor*> ActorsOnTable;




};