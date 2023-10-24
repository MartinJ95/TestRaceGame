// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include <RawMesh/Public/RawMesh.h>
#include "RaceTrackSegment.generated.h"

UCLASS()
class TESTRACEGAME_API ARaceTrackSegment : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARaceTrackSegment();
	void SetMesh(UStaticMesh* newMesh, UMaterialInterface* material);
	void SetPositioning(const ARaceTrackSegment* previous);
	inline void DrawDebugCurve() const;
	inline FVector GetPosition(float& t) const;
	inline void SetID(const int id);
	~ARaceTrackSegment();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	inline void SetStartPosition(const ARaceTrackSegment* previous);
	inline void SetEndPointRotation(const ARaceTrackSegment* previous);
	inline void SetEndPoint(const ARaceTrackSegment* previous, float& sideDir);
	inline void SetControlPoint(const float& sideDir);
	inline void SetDynamicInstance(UMaterialInterface* material);
	inline void UpdateDynamicInstance();
	
protected:
	UPROPERTY()
	UStaticMesh* m_segmentMesh;
	UPROPERTY()
		UStaticMesh* m_segmentDynamicMesh;
	UPROPERTY()
	UStaticMeshComponent* m_meshComp;
	UPROPERTY()
		FVector m_startPoint;
	UPROPERTY()
	FVector m_controlPoint;
	UPROPERTY()
		FVector m_endPoint;
	UPROPERTY()
		FVector m_endPointRotation;
	UPROPERTY()
		UMaterialInstanceDynamic* m_materialInstance;
	UPROPERTY()
		unsigned int m_segmentID;
	static unsigned int currentAmount;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
