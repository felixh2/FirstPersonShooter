// Fill out your copyright notice in the Description page of Project Settings.

#include "WriteToTextHelper.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
/*
UWriteToTextHelper::UWriteToTextHelper()
{
	TextFileName = "TextHelper.txt";
	UE_LOG(LogTemp, Warning, TEXT("Constructor Text Helper"));

	FString WelcomeText = "Welcome To new Debug Tool";
	//Filename = *(FPaths::GameDir() + TextFileName);
	FFileHelper::SaveStringToFile(WelcomeText, *(FPaths::GameDir() + TextFileName));
}
*/
bool UWriteToTextHelper::LoadTextFromFile(FString FileNameA, FString& SaveTextA)
{
	return FFileHelper::LoadFileToString(SaveTextA, *(FPaths::GameDir() + FileNameA));
}

bool UWriteToTextHelper::SaveTextToFile(FString TextToSave)
{
	FString Text;
	FFileHelper::LoadFileToString(Text, *TextFileName);
	Text = Text + "\n" + TextToSave;	
	return FFileHelper::SaveStringToFile(Text, *TextFileName);
}

void UWriteToTextHelper::InitializeTextHelper(FString FileName)
{
/*	UE_LOG(LogTemp, Warning, TEXT("Initialize Text Helper"));
	FString WelcomeText = "Welcome To new Debug Tool";
	Filename = *(FPaths::GameDir() + FileName);
	FFileHelper::SaveStringToFile(WelcomeText, Filename);
	*/
}
