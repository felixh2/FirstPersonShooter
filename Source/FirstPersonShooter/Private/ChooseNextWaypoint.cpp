// Fill out your copyright notice in the Description page of Project Settings.



#include "../Public/ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Classes/AIController.h"

#include "../Public/PatrolRoute.h"

#pragma optimize("", off)
EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Super();
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!ensure(BlackboardComp)) { return EBTNodeResult::Type::Failed; }

	int NextIndex = BlackboardComp->GetValueAsInt(WaypointIndex.SelectedKeyName);
	auto NextWaypoint = BlackboardComp->GetValueAsInt(Waypoint.SelectedKeyName);
	UE_LOG(LogTemp, Warning, TEXT("index - %d"), (int)NextIndex);


	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* AIPAwn = AIController->GetPawn();

	UPatrolRoute* PatrolRoute = AIPAwn->FindComponentByClass<UPatrolRoute>();

	if (!ensure(PatrolRoute)) { return EBTNodeResult::Type::Failed; }
	TArray<AActor*> Waypoints = PatrolRoute->GetPatrolRoute();
	//TArray<AActor*> Waypoints = ((AMyTP_ThirdPersonCharacter*)(AIPAwn))->TargetPoints;   // the Age-Old discussion - we moved to component archetexture

	if (Waypoints.Num() != 0) {
		BlackboardComp->SetValueAsObject(Waypoint.SelectedKeyName, Waypoints[NextIndex]);
		/*
		TArray < FBlackboardEntry > Keys = Blackboard->GetBlackboardAsset()->Keys;
		FBlackboard::FKey KeyIndex =  Blackboard->GetKeyID(FName("NextWaypointIndex"));
		UE_LOG(LogTemp, Warning, TEXT("Entry name - %s"), *(Keys[KeyIndex].EntryName).ToString());
		*/



		NextIndex++;
		NextIndex = NextIndex % (Waypoints.Num());

		BlackboardComp->SetValueAsInt(WaypointIndex.SelectedKeyName, NextIndex);
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing waypoints"));
	}
	return EBTNodeResult::Type::Succeeded;
}













void UChooseNextWaypoint::set()
{
}



#pragma optimize("", on)