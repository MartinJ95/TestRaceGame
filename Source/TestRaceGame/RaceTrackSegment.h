// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "RaceTrackSegment.generated.h"

UCLASS()
class TESTRACEGAME_API ARaceTrackSegment : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARaceTrackSegment();
	void SetMesh(UStaticMesh* newMesh);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY()
	UStaticMesh* m_segmentMesh;
	UPROPERTY()
	UStaticMeshComponent* m_meshComp;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
