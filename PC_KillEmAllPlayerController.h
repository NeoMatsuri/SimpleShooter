// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PC_KillEmAllPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API APC_KillEmAllPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;
	
	 APC_KillEmAllPlayerController();

protected:

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float RestartDelay = 5.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HUDClass;


	FTimerHandle RestartTimer;

	UPROPERTY()
	UUserWidget* HUD;
};
