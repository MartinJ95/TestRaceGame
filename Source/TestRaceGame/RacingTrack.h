// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RaceTrackSegment.h"
#include "RacingTrack.generated.h"

USTRUCT()
struct FEditorFunction
{
	GENERATED_BODY()
public:
	FEditorFunction() : m_shouldRun(false), m_function([](void* ptr) {}), ownerPtr(nullptr) {}
	FEditorFunction(const std::function<void(void*)>& function, void* owner) : m_shouldRun(false), m_function(function), ownerPtr(owner) {}
	void CheckRunFunction()
	{
		if (m_shouldRun)
		{
			m_function(ownerPtr);
			m_shouldRun = false;
		}
	}
protected:
	UPROPERTY(EditAnywhere)
		bool m_shouldRun;
	UPROPERTY()
		std::function<void(void*)> m_function = [](void* ptr) {};
	UPROPERTY()
		void* ownerPtr;
};


UCLASS()
class TESTRACEGAME_API ARacingTrack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARacingTrack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	TArray<ARaceTrackSegment*> m_segments;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};