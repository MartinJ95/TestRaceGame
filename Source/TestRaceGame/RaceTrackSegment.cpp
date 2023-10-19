// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceTrackSegment.h"

// Sets default values
ARaceTrackSegment::ARaceTrackSegment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("segmentMesh"));
	m_meshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void ARaceTrackSegment::SetMesh(UStaticMesh* newMesh)
{
	m_segmentMesh = newMesh;
	if (m_segmentMesh == nullptr)
		return;

	m_meshComp->SetStaticMesh(m_segmentMesh);
}

void ARaceTrackSegment::SetPositioning(ARaceTrackSegment* previous)
{
	SetStartPosition(previous);
}

// Called when the game starts or when spawned
void ARaceTrackSegment::BeginPlay()
{
	Super::BeginPlay();
	
}

inline void ARaceTrackSegment::SetStartPosition(ARaceTrackSegment* previous)
{
	if (previous == nullptr)
	{
		m_startPoint = FVector::ZeroVector;
		return;
	}
	m_startPoint = previous->m_endPoint;
}

// Called every frame
void ARaceTrackSegment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

