// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "MyTP_ThirdPersonCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSONSHOOTER_API AMyTP_ThirdPersonCharacter : public ATP_ThirdPersonCharacter
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Waypoints")
	TArray<AActor*> TargetPoints;
	
	
};
