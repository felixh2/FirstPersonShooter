// Fill out your copyright notice in the Description page of Project Settings.



#include "../Public/ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Super();
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!ensure(BlackboardComp)) { return EBTNodeResult::Type::Failed; }

	auto NextIndex = BlackboardComp->GetValueAsInt(WaypointIndex.SelectedKeyName);
	auto NextWaypoint = BlackboardComp->GetValueAsInt(Waypoint.SelectedKeyName);
	UE_LOG(LogTemp, Warning, TEXT("index - %d"), (int)NextIndex);
	/*
	TArray < FBlackboardEntry > Keys = Blackboard->GetBlackboardAsset()->Keys;
	FBlackboard::FKey KeyIndex =  Blackboard->GetKeyID(FName("NextWaypointIndex"));
	UE_LOG(LogTemp, Warning, TEXT("Entry name - %s"), *(Keys[KeyIndex].EntryName).ToString());
	*/
	


	


	
	return EBTNodeResult::Type::Succeeded;
}













void UChooseNextWaypoint::set()
{
}


