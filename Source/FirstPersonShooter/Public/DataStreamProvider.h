// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "E:/Unreal Projects/FirstPersonShooter/ThirdParty/UE4_staticLibrary/Includes/libusb.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Delegates/Delegate.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataStreamProvider.generated.h"

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


//class UWeapon;


UCLASS(ClassGroup = (DataStream), meta = (BlueprintSpawnableComponent))
class FIRSTPERSONSHOOTER_API ADataStreamProvider : public AActor
{
	GENERATED_BODY()

public:
	ADataStreamProvider();

protected:
	virtual void BeginPlay() override;

	//UPROPERTY(BlueprintAssignable)
	//FOnFireRequest OnFireRequest;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void InitDataStreamProvider();

	UPROPERTY(BlueprintReadOnly)
		float Fire;

	// TODO : move from here
//	UFUNCTION(BlueprintCallable, Category = Setup)
//		void SetWeaponReference(UWeapon* WeaponRef);

	UFUNCTION(BlueprintCallable, Category = Setup)
		void ConnectToDataStream();

	//UWeapon* Weapon = nullptr;
	//void WeaponDrawDebugLine();
	///////////////////////////


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

	float angle_X = 0;
	float angle_Y = 0;
	float angle_Z = 0;

	bool IsConnected = true;

};
