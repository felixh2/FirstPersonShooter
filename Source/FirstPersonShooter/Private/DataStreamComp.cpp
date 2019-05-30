// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/DataStreamComp.h"
#include "TimerManager.h"
#include "FirstPersonShooterCharacter.h"

#pragma optimize("", off)
// Sets default values for this component's properties
UDataStreamComp::UDataStreamComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDataStreamComp::BeginPlay()
{
	Super::BeginPlay();
	auto Owner = GetOwner();
	auto RootComp = Owner->GetRootComponent();

	TP_CharacterComp= Cast<AFirstPersonShooterCharacter>(GetOwner());
	if (!ensure(TP_CharacterComp)) { return; }
	UE_LOG(LogTemp, Warning, TEXT("Data Stream component reporting"))
	ConnectToDataStream();
		//GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &UDataStreamComp::GetDataStream, 0.04f, true, -1.f);
	
}

// Called every frame
void UDataStreamComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsConnected) GetDataStream();
}

int UDataStreamComp::Initlibusb(int VENDOR_ID, int PRODUCT_ID) {
	vendorId = VENDOR_ID;
	productId = PRODUCT_ID;

	rc = libusb_init(NULL);
	if (rc < 0) {
		//fprintf(stderr, "Error initializing libusb: %s\n", libusb_error_name(rc));
		return -1;
	}
	return 1;
}

void UDataStreamComp::SetDebug(int level) {

	/* Set debugging output to max level.
	*/
	libusb_set_debug(NULL, 3);
}

int UDataStreamComp::OpenDevice()
{
	devh = libusb_open_device_with_vid_pid(NULL, vendorId, productId);
	if (!devh) {
		fprintf(stderr, "Error finding USB device\n");
		return -1;
	}
	return  0;
}

int UDataStreamComp::AttachDetachKernel()
{
	/* As we are dealing with a CDC-ACM device, it's highly probable that
	* Linux already attached the cdc-acm driver to this device.
	* We need to detach the drivers from all the USB interfaces. The CDC-ACM
	* Class defines two interfaces: the Control interface and the
	* Data interface.
	*/
	for (int if_num = 0; if_num < 2; if_num++) {
		if (libusb_kernel_driver_active(devh, if_num)) {
			libusb_detach_kernel_driver(devh, if_num);

		}
		rc = libusb_claim_interface(devh, if_num);
		if (rc < 0) {
			fprintf(stderr, "Error claiming interface: %s\n",
				libusb_error_name(rc));
			return -1;
		}
		else if (rc >= 0) {
			UE_LOG(LogTemp, Warning, TEXT("libusb interface claimed: %d"), if_num);
		}
	}
	return 1;
}

int UDataStreamComp::ReadChars(unsigned char *data, int size)
{
	/* To receive characters from the device initiate a bulk_transfer to the
	* Endpoint with address ep_in_addr.
	*/
	int actual_length;
	int rc = libusb_bulk_transfer(devh, ep_in_addr, data, size, &actual_length,
		1000);
	if (rc == LIBUSB_ERROR_TIMEOUT) {
		printf("timeout (%d)\n", actual_length);
		return -1;
	}
	else if (rc < 0) {
		fprintf(stderr, "Error while waiting for char\n");
		return -1;
	}

	return actual_length;
}

void UDataStreamComp::SetLineState()
{
	/* Start configuring the device:
	* - set line state
	*/
	rc = libusb_control_transfer(devh, 0x21, 0x22, ACM_CTRL_DTR | ACM_CTRL_RTS,
		0, NULL, 0, 0);
	if (rc < 0) {
		fprintf(stderr, "Error during control transfer: %s\n",
			libusb_error_name(rc));
	}

}

void UDataStreamComp::SetLineEncoding()
{
	/* - set line encoding: here 9600 8N1
	* 9600 = 0x2580 ~> 0x80, 0x25 in little endian
	*/
	unsigned char encoding[] = { 0x80, 0x25, 0x00, 0x00, 0x00, 0x00, 0x08 };
	//unsigned char encoding[] = { 0x00, 0xc2, 0x01, 0x00, 0x00, 0x00, 0x08 };
	rc = libusb_control_transfer(devh, 0x21, 0x20, 0, 0, encoding,
		sizeof(encoding), 0);
	if (rc < 0) {
		fprintf(stderr, "Error during control transfer: %s\n",
			libusb_error_name(rc));
	}

}

void UDataStreamComp::TerminateLibusb() {

	libusb_close(devh);
	//libusb_exit(NULL); UE Crashes
	UE_LOG(LogTemp, Warning, TEXT("libusb terminated"));

}

void UDataStreamComp::GetDataStream() {

	//GeneralTimerCurrTime = GetWorldTimerManager().GetTimerElapsed(GeneralTimer);
	//GeneralTimerduration = GeneralTimerCurrTime - GeneralTimerPrevTime;
	//UE_LOG(LogTemp, Warning, TEXT("duration of UpdateRotation is %f"), duration);
	
	

	float PrevAngle_X = angle_X;
	float PrevAngle_Y = angle_Y;
	float PrevAngle_Z = angle_Z;

	if (IsConnected)
	{
		ReadChars(buf, BUFFER_SIZE);
		angle_X = *(float *)&buf;
		angle_Y = *(float*)(buf + 4);
		angle_Z = *(float*)(buf + 8);

		Fire = *(float *)(buf + 12);


		

		((APawn*)TP_CharacterComp)->AddControllerYawInput(-(angle_Z - PrevAngle_Z) / YawScale);
		((APawn*)TP_CharacterComp)->AddControllerPitchInput((angle_X - PrevAngle_X) / PitchScale);
		((APawn*)TP_CharacterComp)->AddControllerRollInput(-(angle_Y - PrevAngle_Y) / RollScale);

		if ((Fire != 0) & ((GetWorld()->GetTimeSeconds()) - CurrTime > ReloadTime))
		{
			TP_CharacterComp->OnFire();
			CurrTime = GetWorld()->GetTimeSeconds();
		}

		UE_LOG(LogTemp, Warning, TEXT("angleX is : %f, angleY is : %f, angleZ is : %f"), angle_X, angle_Y, angle_Z);

		/*
		FRotator ActorRotation = this->GetActorRotation();
		ActorRotation.Pitch = -angle_X;
		ActorRotation.Roll = -angle_Y;
		ActorRotation.Yaw = -angle_Z;
		SetActorRotation(ActorRotation);
		*/

	}



	//GeneralTimerPrevTime = GetWorldTimerManager().GetTimerElapsed(GeneralTimer);
}

void UDataStreamComp::ConnectToDataStream()
{
	int ARDUINO_VENDOR_ID = 0x2341;
	int ARDUINO_PRODUCT_ID = 0x0042;

	int STM32_VENDOR_ID = 0x0483;
	int STM32_PRODUCT_ID = 0x5740;


	int ans = Initlibusb(STM32_VENDOR_ID, STM32_PRODUCT_ID);
	if (ans == 1) {

		UE_LOG(LogTemp, Warning, TEXT("libusb initialized"));
		// Critical to ensure that openDevice is equal to zero
		// otherwise, the UE4 editor will crash when the usb device isn't connected. 
		if (OpenDevice() == 0) {
			SetDebug(3);
			AttachDetachKernel();
			SetLineState();
			SetLineEncoding();
			UE_LOG(LogTemp, Warning, TEXT("libusb device opened"));
			IsConnected = true;

		}
	}
}


// TODO: move from here
/*
void ADataStreamProvider::WeaponDrawDebugLine() {

FVector Start = Weapon->GetSocketLocation(FName("FireNozzle"));
FVector End = Start + Weapon->GetSocketRotation(FName("FireNozzle")).Vector() * 10000;
DrawDebugLine(
GetWorld(),
Start,
End,
FColor(0, 0, 255),
false,
0.f,
0.f,
5.f
);
}
*/

/*
void ADataStreamProvider::SetWeaponReference(UWeapon * WeaponRef)
{
Weapon = WeaponRef;
}
*/

#pragma optimize("", on)

