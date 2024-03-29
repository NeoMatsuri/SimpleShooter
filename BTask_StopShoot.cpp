// Fill out your copyright notice in the Description page of Project Settings.


#include "BTask_StopShoot.h"
#include "AIController.h"
#include "ShooterCharacter.h"

UBTask_StopShoot::UBTask_StopShoot()
{
    NodeName = TEXT("StopShoot");
}

EBTNodeResult::Type UBTask_StopShoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

   if (OwnerComp.GetAIOwner() == nullptr)
   {
       return EBTNodeResult::Failed;
   }

   AShooterCharacter* Character = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
   if (Character == nullptr)
   {
       return EBTNodeResult::Failed;
   }

        Character->StopShoot();
    
    return EBTNodeResult::Succeeded;
};
