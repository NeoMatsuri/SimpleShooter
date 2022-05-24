// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	void PullTrigger();
	void Reload();
	void AddAmmo();
	
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	float GetAmmoCount() const;

	int GetClipCount() const;
	
private:

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
	USoundBase* EmptyClip;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere)
	float MaxRange = 10000;

	UPROPERTY(EditAnywhere)
	int MaxAmmoRifle = 120;

	UPROPERTY(EditAnywhere)
	int RifleClipMax = 30;

	UPROPERTY(EditAnywhere)
	int AmmoCrateIncrease = 30;

	UPROPERTY(EditAnywhere)
	int CurrentAmmoRifle = MaxAmmoRifle;
	
	UPROPERTY(EditAnywhere)
	int RifleClip = RifleClipMax;

	UPROPERTY(EditAnywhere)
	float Damage = 10.f;

	int Difference = RifleClipMax - RifleClip;
	
	int ReturnCurrentAmmoRifle = CurrentAmmoRifle;

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

	AController* GetOwnerController() const;

	

};
