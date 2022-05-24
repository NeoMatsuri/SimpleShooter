// Fill out your copyright notice in the Description page of Project Settings.


#include "BTask_Reload.h"
#include "AIController.h"
#include "ShooterCharacter.h"

UBTask_Reload::UBTask_Reload()
{
    NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTask_Reload::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

        Character->Reload();
    
    return EBTNodeResult::Succeeded;
}