// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cover.generated.h"

UCLASS()
class SIMPLESHOOTER_API ACover : public AActor
{
	GENERATED_BODY()

private:

	UFUNCTION()
		void OnCompBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnCompEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

		/** Returns true if the socket is close to the player */
	bool IsCloseToPlayer(FName SocketName);

	/** Determines the movement direction and the facing direction of the player */
	void DetermineMovementDirection(FVector& MovementDirection, FRotator& FacingDirection);

	/** Returns the name of the nearby socket */
	FName GetNearbySocket();
	
	
public:	
	// Sets default values for this actor's properties
	ACover();
	virtual void Tick(float DeltaTime) override;
	/** Retrieves the movement direction and the facing rotation of the player */
	void RetrieveMovementDirectionAndFacingRotation(FVector& MovementDirection, FRotator& FacingRotation);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** The box component that informs the player if he's able to take cover or not */
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SM;

};
