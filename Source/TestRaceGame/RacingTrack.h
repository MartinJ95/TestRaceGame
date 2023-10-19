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
	FEditorFunction();
	FEditorFunction(void* owner);
	FEditorFunction(const std::function<void(void* ptr)>& function, void* owner);
	void CheckRunFunction();
protected:
	UPROPERTY(EditAnywhere)
		bool m_shouldRun;
		std::function<void(void* ptr)> m_function = [](void* ptr) {};
		void* ptr;
};


UCLASS()
class TESTRACEGAME_API ARacingTrack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARacingTrack();
	UStaticMesh* GetMesh() const;
	TArray<ARaceTrackSegment*>& GetSegments();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	TArray<ARaceTrackSegment*> m_segments;
	UPROPERTY(EditAnywhere)
	UStaticMesh* m_pathSegmentMesh;
#if WITH_EDITOR
	UPROPERTY(EditAnywhere)
	FEditorFunction m_addSegment;
	UPROPERTY(EditAnywhere)
		FEditorFunction m_positionSegments;
#endif
	bool m_editorRunning = false;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool ShouldTickIfViewportsOnly() const override;

};