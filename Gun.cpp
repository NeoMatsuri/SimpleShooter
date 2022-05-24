// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"


// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);


}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmoRifle = MaxAmmoRifle;
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AGun::GetAmmoCount() const
{
	return FMath::Clamp(CurrentAmmoRifle, 0, MaxAmmoRifle);;
}

int AGun::GetClipCount() const
{
	return RifleClip;
}



bool AGun::GunTrace(FHitResult &Hit, FVector &ShotDirection)
{
	AController *OwnerController = GetOwnerController();
	if(OwnerController == nullptr) return false;
	//Declaring the Location and Rotation Variables and then defining them by using the GetPlayerViewpoint() which returns an FVector and FRotator.
	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector(); //Calculating the Direction of the shot 

	//Declaring the End variable which is effectively providing the Range for our Gun. 
	FVector End = Location + Rotation.Vector() * MaxRange;
	
	//Declaring the HitResult variable which we will need later to DrawDebugPoint.
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	
	//Declaring the bSuccess Varaible. LineTraceSingleByChannel will be providing the definition for out HitResult variable and we pass in our Location and End variable from before.
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	//Getting the Owner of the Gun and casting it to be of type APawn. We Cast because the gun is of type AActor
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr) return nullptr; 
	//Getting the Controller for the OwnerPawn. Which again, we can do because we cast GetOwner to APawn
	return OwnerPawn->GetController();
}

void AGun::PullTrigger()
{
	
	if (RifleClip > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Ammo is %i"), RifleClip);
		--RifleClip;
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));
	
	FHitResult Hit; 
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection);
	//Finally we if we confirm a hit we call various functions. 
	if(bSuccess)
	{	
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactEffect, Hit.Location, ShotDirection.Rotation()); //Spawning the particle emitter at the location of the hit
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);

		//if hit actor does not equal a nullptr then we call take damage
		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			AController *OwnerController = GetOwnerController();
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
		//DrawDebugPoint(GetWorld(), HitResult.Location, 20, FColor::Red, true);
	}
	
	} 

	else 
	{
		UGameplayStatics::SpawnSoundAttached(EmptyClip, Mesh, TEXT("MuzzleFlashSocket"));
		return;
	}
	
}

void AGun::Reload()
{
	Difference = RifleClipMax - RifleClip;
	Difference = FMath::Clamp(Difference, 1, CurrentAmmoRifle);
	if (RifleClip < RifleClipMax && CurrentAmmoRifle > 0)
	{
	RifleClip = RifleClip + Difference;
	CurrentAmmoRifle = CurrentAmmoRifle - Difference;
	UE_LOG(LogTemp, Warning, TEXT("Current Ammo is %i"), RifleClip);
	UE_LOG(LogTemp, Warning, TEXT("Difference Is %i"), Difference);
	UE_LOG(LogTemp, Warning, TEXT("Ammo in Reserves is %i"), CurrentAmmoRifle);
	}

	else return;
}

void AGun::AddAmmo()
{
	CurrentAmmoRifle += AmmoCrateIncrease;
	CurrentAmmoRifle = FMath::Clamp(CurrentAmmoRifle, 0, MaxAmmoRifle);
	UE_LOG(LogTemp, Warning, TEXT("Ammo in Reserves is %i"), CurrentAmmoRifle);
}

