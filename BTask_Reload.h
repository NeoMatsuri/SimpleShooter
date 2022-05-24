// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTask_Reload.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTask_Reload : public UBTTaskNode
{
	GENERATED_BODY()

public: 

    UBTask_Reload();

protected: 

virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
