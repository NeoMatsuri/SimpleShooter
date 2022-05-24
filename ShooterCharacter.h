// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.h"
#include "Cover.h"
#include "ShooterCharacter.generated.h"


class AShooterAIController;
class AGun;
class AAmmoPickup;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	AShooterCharacter();

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure)
	float GetAmmoCount() const;

	UFUNCTION(BlueprintPure)
	int GetClipCount() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Shoot();
	void StopShoot();
	void GunShoot();
	void Reload();
	void AddAmmo();

	UFUNCTION()
		void CrouchStart();
	
	UFUNCTION()
		void ToggleCover();
	bool bIsShooting = false; 
	

	void SetCanTakeCover(bool bCanTakeCover, ACover* CoverActor);
private:

	bool bCanTakeCover = false;

	bool bIsInCover = false;

	ACover* Cover;

	void MoveForward(float AxisValue);
	void MoveSideways(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookSidewaysRate(float AxisValue);
	void SwitchWeapons(float Slot);
	
	UPROPERTY(EditAnywhere)
	float RotationRate = 10.f;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere)
	float Health;
	
	
	UPROPERTY(EditAnywhere)
	int32 ActiveIndex;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass[3];
	UPROPERTY()
	AGun* Gun[3];

	FTimerHandle TimerHandle_HandleReFire;

	float TimeBetweenShots = .1f;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAmmoPickup> AmmoPickupClass;
	UPROPERTY()
	AAmmoPickup* AmmoPickup;

	AShooterAIController* AIController;

	bool bCanSpawnItem = false;
	bool HasCheckedDead = false;

	FVector CoverDirectionMovement;
};
