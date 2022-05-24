// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTask_Shoot.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTask_Shoot : public UBTTaskNode
{
	GENERATED_BODY()
	
public: 

    UBTask_Shoot();

protected: 

virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
