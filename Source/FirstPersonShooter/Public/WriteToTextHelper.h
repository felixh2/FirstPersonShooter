// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WriteToTextHelper.generated.h"

/**
 * Helper class for saving text data to file
 */
UCLASS()
class FIRSTPERSONSHOOTER_API UWriteToTextHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	//UWriteToTextHelper();

	UFUNCTION(BlueprintPure, Category = "Custom", meta = (Keywords = "Load Text From File"))
	static bool LoadTextFromFile(FString FileNameA, FString& SaveTextA);

	UFUNCTION(BlueprintCallable, Category = "Custom", meta = (Keywords = "Save Text To File"))
	static bool SaveTextToFile(FString TextToSave);

	UFUNCTION(BlueprintCallable, Category = "Custom", meta = (Keywords = "SaveTxt"))
	static void InitializeTextHelper(FString FileName);
	

	static FString TextFileName;
	static  const TCHAR* Filename;
};
