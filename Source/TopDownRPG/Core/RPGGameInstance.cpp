// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGGameInstance.h"

#include "MoviePlayer.h"

void URPGGameInstance::BeginLoadingScreen()
{
	FLoadingScreenAttributes LoadingScreenAttributes;
	LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
	UUserWidget* UserWidget = CreateWidget<UUserWidget>(GetWorld(), LoadingScreen);
	UserWidget->AddToViewport(100);
	
	LoadingScreenAttributes.WidgetLoadingScreen = UserWidget->TakeWidget();
	LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 3;
	
	GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
}
