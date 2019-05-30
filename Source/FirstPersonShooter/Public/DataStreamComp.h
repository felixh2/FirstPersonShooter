// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "E:/Unreal Projects/FirstPersonShooter/ThirdParty/UE4_staticLibrary/Includes/libusb.h"
#include "Runtime/Engine/Classes/Engine/World.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataStreamComp.generated.h"

////////////////// defines//////////////
#define BUFFER_SIZE 16
#define JUMP_VALUE 8
#define WAIT_PERIOD 5
#define ACM_CTRL_DTR   0x01
#define ACM_CTRL_RTS   0x02
////////////////// defines//////////////



// Arduino
//static int ep_in_addr = 0x83;
//static int ep_out_addr = 0x02;

// STM32
static int ep_in_addr = 0x81;
static int ep_out_addr = 0x02;


class AFirstPersonShooterCharacter;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTPERSONSHOOTER_API UDataStreamComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDataStreamComp();

	UFUNCTION(BlueprintCallable, Category = Setup)
	void ConnectToDataStream();

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float ReloadTime = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Scaling")
	float YawScale = 2.f;

	UPROPERTY(EditAnywhere, Category = "Scaling")
	float PitchScale = 1.f;

	UPROPERTY(EditAnywhere, Category = "Scaling")
	float RollScale = 1.f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	int  Initlibusb(int, int);
	void SetDebug(int level);
	int  OpenDevice();
	int  AttachDetachKernel();
	int  ReadChars(unsigned char * data, int size);
	void SetLineState();
	void SetLineEncoding();
	void TerminateLibusb();
	void GetDataStream();
		
private:
	AFirstPersonShooterCharacter* TP_CharacterComp = nullptr;
	struct libusb_device_handle *devh = NULL;
	int vendorId;
	int productId;
	int rc;
	unsigned char buf[BUFFER_SIZE] = { 0 };

	FTimerHandle CountdownTimerHandle;
	FTimerHandle GeneralTimer;
	float GeneralTimerPrevTime = 0.0;
	float GeneralTimerCurrTime = 0.0;
	float GeneralTimerduration = 0.0;


	float CurrTime = 0.f;

	float angle_X = 0;
	float angle_Y = 0;
	float angle_Z = 0;
	float Fire;

	bool IsConnected = true;
};
