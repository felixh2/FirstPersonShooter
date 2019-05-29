// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/PatrolRoute.h"


// Sets default values for this component's properties
UPatrolRoute::UPatrolRoute()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


TArray<AActor*> UPatrolRoute::GetPatrolRoute()
{
	return PatrolPoints;
}

// Called when the game starts
void UPatrolRoute::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


