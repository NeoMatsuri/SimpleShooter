// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterAIController.h"
#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"
#include "AmmoPickup.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	ActiveIndex = 0; 
	UE_LOG(LogTemp, Warning, TEXT("ActiveIndex = %d"), ActiveIndex);
	for (int32 Index = 0; Index < 3; Index++)
	{
		Gun[Index] = GetWorld()->SpawnActor<AGun>(GunClass[Index]);

		if(Gun[Index])
		{
			Gun[Index]->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
			Gun[Index]->SetOwner(this);
			UE_LOG(LogTemp, Warning, TEXT("Attached Mesh Number is %d"), Index);

			if (Index != ActiveIndex)
			{
				Gun[Index]->SetActorHiddenInGame(true);
			}
	
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Gun Attached on %d"), Index);
		}
	}
	
	 AIController = Cast<AShooterAIController>(GetController());
	 AIController->SetAmmoKey();
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}



// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("MoveSideways"), this, &AShooterCharacter::MoveSideways);
	PlayerInputComponent->BindAxis(TEXT("LookSideways"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAxis(TEXT("MoveSideways"), this, &AShooterCharacter::MoveSideways);
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &AShooterCharacter::CrouchStart);
	PlayerInputComponent->BindAction(TEXT("TakeCover"), EInputEvent::IE_Pressed, this, &AShooterCharacter::ToggleCover);
	PlayerInputComponent->BindAxis(TEXT("LookSidewaysRate"), this, &AShooterCharacter::LookSidewaysRate);
	PlayerInputComponent->BindAction(TEXT("PullTrigger"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("ReleaseTrigger"), EInputEvent::IE_Released, this, &AShooterCharacter::StopShoot);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Reload);
	PlayerInputComponent->BindAxis(TEXT("SwitchWeapons"), this, &AShooterCharacter::SwitchWeapons);
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) 
 {
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageApplied = FMath::Min(Health, DamageApplied);
	Health -= DamageApplied;
	UE_LOG(LogTemp, Warning, TEXT("Current Health: %f"), Health);
	

	if (IsDead() && !HasCheckedDead)
	{
		HasCheckedDead = true;
		bCanSpawnItem = true;
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if(GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		FVector DeadLocation = GetActorLocation();
		FRotator DeadRotation = GetActorRotation();
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if(bCanSpawnItem)
		{
		AmmoPickup = GetWorld()->SpawnActor<AAmmoPickup>(AmmoPickupClass, DeadLocation, DeadRotation);
		bCanSpawnItem = false;
		}
		StopShoot();
	}

	return DamageApplied;
 }

void AShooterCharacter::MoveForward(float AxisValue)
{
	//We don't want to move forward or backwards when we're taking cover
	if ((Controller != NULL) && (AxisValue != 0.0f) && !bIsInCover)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
 
		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, AxisValue);
	}
	//AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveSideways(float AxisValue)
{
	if ((Controller != NULL) && (AxisValue != 0.0f))
	{
		if (!bIsInCover)
		{
			//default movement functionality
 
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
 
			// get right vector 
			FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, AxisValue);
		}
		else
		{
			//Move according to the cover actor's position
			AddMovementInput(CoverDirectionMovement, AxisValue);
		}
	}
	//AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds()); 
}

void AShooterCharacter::LookSidewaysRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::SwitchWeapons(float Slot)
{
	int32 SlotIndex = static_cast<int32>(Slot);
	Gun[ActiveIndex]->SetActorHiddenInGame(true);
	ActiveIndex += SlotIndex;
	ActiveIndex = FMath::Clamp(ActiveIndex, 0, 2);
	Gun[ActiveIndex]->SetActorHiddenInGame(false);
}

void AShooterCharacter::Shoot()
{
	Gun[ActiveIndex]->PullTrigger();
	GetWorldTimerManager().SetTimer(TimerHandle_HandleReFire, this, &AShooterCharacter::GunShoot, TimeBetweenShots, true);
	bIsShooting = true; 
}

void AShooterCharacter::StopShoot()
{
	bIsShooting = false; 
	GetWorldTimerManager().ClearTimer(TimerHandle_HandleReFire);
}

void AShooterCharacter::GunShoot()
{
	Gun[ActiveIndex]->PullTrigger();
}

void AShooterCharacter::Reload()
{
	if (bIsShooting) return; 

	else
	{
		Gun[ActiveIndex]->Reload();
	}
}

float AShooterCharacter::GetAmmoCount() const
{
	return Gun[ActiveIndex]->GetAmmoCount();
}

int AShooterCharacter::GetClipCount() const
{
	return Gun[ActiveIndex]->GetClipCount();
}

void AShooterCharacter::AddAmmo()
{
	UE_LOG(LogTemp, Warning, TEXT("You did it Shinji"));
	Gun[ActiveIndex]->AddAmmo();
}

void AShooterCharacter::CrouchStart()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	
	else 
	{
		Crouch();
	}
}

void AShooterCharacter::ToggleCover()
{
	if (GetCharacterMovement()->IsCrouching() && bCanTakeCover)
	{
		
		bIsInCover = !bIsInCover;
 
		if (bIsInCover && Cover)
		{
			//This is done because my downloaded animations do not require an orientation to movement
			//Depending on your animation you may (or not) need this
			GetCharacterMovement()->bOrientRotationToMovement = false;
 
			FRotator CoverRotation;
			Cover->RetrieveMovementDirectionAndFacingRotation(CoverDirectionMovement, CoverRotation);
			SetActorRotation(CoverRotation);
		}	
		else
		{
			//This is done because my downloaded animations do not require an orientation to movement
			//Depending on your animation you may (or not) need this
			GetCharacterMovement()->bOrientRotationToMovement = true;
		}
	}
}

void AShooterCharacter::SetCanTakeCover(bool CanTakeCover, ACover* CoverActor)
{
	if (!bCanTakeCover && bIsInCover)
	{
		ToggleCover();
	}

	bCanTakeCover = CanTakeCover;
	Cover = CoverActor;
}



	
 