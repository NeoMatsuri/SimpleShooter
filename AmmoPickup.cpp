// Fill out your copyright notice in the Description page of Project Settings.



#include "ShooterCharacter.h"
#include "AmmoPickup.h"
#include "Gun.h"




// Sets default values
AAmmoPickup::AAmmoPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	AmmoCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("AmmoCollider"));
	AmmoCollider->SetupAttachment(Root);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoBoxMesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AAmmoPickup::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmoPickup::OnOverlapBegin);
	Mesh->AddImpulse(FVector(FMath::RandRange(500, 3000),FMath::RandRange(500, 3000),FMath::RandRange(500, 3000)));
}

// Called every frame
void AAmmoPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAmmoPickup::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{	
	UE_LOG(LogTemp, Warning, TEXT("The Actor that got hit is %s"), OtherActor);
}

void AAmmoPickup::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

if(auto* PCharacter = Cast<AShooterCharacter>(OtherActor))
{
	PCharacter->AddAmmo();
	Destroy();
}

	
}



