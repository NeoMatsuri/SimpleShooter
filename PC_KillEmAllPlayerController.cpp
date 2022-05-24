// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_KillEmAllPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"



APC_KillEmAllPlayerController::APC_KillEmAllPlayerController()
{
    
}

void APC_KillEmAllPlayerController::BeginPlay()
{
    HUD = CreateWidget(this, HUDClass);
    if(HUD != nullptr)
    {
        HUD->AddToViewport();
    }
}


void APC_KillEmAllPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    HUD->RemoveFromViewport();
    if (bIsWinner)
    {
        UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
        if(WinScreen != nullptr)
        {
            WinScreen->AddToViewport();
        }
    }
    else
    {
        UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
        if(LoseScreen != nullptr)
        {
            LoseScreen->AddToViewport();
        }
    }
    

    GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
    
}

