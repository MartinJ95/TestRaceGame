// Fill out your copyright notice in the Description page of Project Settings.


#include "RacingTrack.h"

FEditorFunction::FEditorFunction() :
	m_shouldRun(false), m_function([](void* ptr) {}), ptr(nullptr) {}


FEditorFunction::FEditorFunction(void* owner) :
	m_shouldRun(false), m_function([](void* ptr) {}), ptr(owner) {}

FEditorFunction::FEditorFunction(const std::function<void(void* ptr)>& function, void* owner) :
	m_shouldRun(false), m_function(function), ptr(owner) {}

void FEditorFunction::CheckRunFunction()
{
	if (m_shouldRun)
	{
		m_function(ptr);
		m_shouldRun = false;
	}
}

// Sets default values
ARacingTrack::ARacingTrack() : m_segments()
#if WITH_EDITOR 
, m_addSegment(
	[](void* ptr)
	{
		ARacingTrack* track = static_cast<ARacingTrack*>(ptr);
		if (track == nullptr)
			return;
		track->m_segments.Emplace(track->GetWorld()->SpawnActor<ARaceTrackSegment>(ARaceTrackSegment::StaticClass()));
		
	}, this)
#endif
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
#if WITH_EDITOR
	PrimaryActorTick.bStartWithTickEnabled = true;
	m_editorRunning = true;
#endif
}


// Called when the game starts or when spawned
void ARacingTrack::BeginPlay()
{
	Super::BeginPlay();
	m_editorRunning = false;
}

// Called every frame
void ARacingTrack::Tick(float DeltaTime)
{
#if WITH_EDITOR
	if (m_editorRunning)
	{
		m_addSegment.CheckRunFunction();
		return;
	}
#endif
	Super::Tick(DeltaTime);
}

bool ARacingTrack::ShouldTickIfViewportsOnly() const
{
	return true;
}

